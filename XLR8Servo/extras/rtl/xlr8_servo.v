/////////////////////////////////
// Filename    : xlr8_servo.v
// Author      : Matt Weber
// Description : A collection of servo channels (up to 32) and the
//                AVR IO registers needed to access them.
//               The servos aren't completely independant. They are backed
//                by 16 timers and directly alias (e.g. servo[0] and [16] always
//                have the same pulse width, although they can be separately
//                enabled/disabled).
//                3 Registers
//                  ControlRegister : 
//                        [7]   = enable channel (strobe write, read=enable for selected channel)
//                        [6]   = disable channel (strobe, always read as zero)
//                        [5]   = update channel pulse width (strobe, always read as zero)
//                        [4:0] = servo channel to enable/disable/update
//                  PulseWidthL :  [7:0]= lower 8 bits of servo pulse width in microseconds
//                  PulseWidthH :  [3:0]= upper 4 bits of servo pulse width in microseconds
//                   There is a PulseWidth register pair for each servo channel. 
//                    When written, the channel to access is given in ControlReg[4:0].
//                    On write it sets the pulse width that will be set when the control
//                      register is written with the update bit set
//                    Read just returns the last value written, regardless of ControlReg[4:0]
//                   To start a channel typically the PulseWidth registers would be written
//                    first, then the control register with the desired channel indicated and
//                    both the enable and update bits set
//               Future work. It should be possible to measure pulse widths with very little
//                 additional hardware. On the rising edge of of a pin to be measured, store
//                 the value of timercnt into chan_pw[pin]. Then on the falling edge of the
//                 pin do chan_pw[pin] = timercnt - chan_pw[pin] and adjust for wrapping.
//                 For the pin that is selected by ControlReg[4:0], the value of chan_pw[pin]
//                 is copied into the PulseWidthH/L registers, except that while chan_pw[pin]
//                 is holding the rising edge time (i.e. the pin is high) PulseWidthH[7] is
//                 also set and the software library can use this to determine that it needs to
//                 read the register again to get a valid result.       
//
// Copyright 2015, Superion Technology Group. All Rights Reserved
/////////////////////////////////

module xlr8_servo
 #(parameter NUM_SERVOS = 12,
   parameter SVCR_ADDR  = 6'h0, // servo control register
   parameter SVPWH_ADDR = 6'h0, // servo pulse width high
   parameter SVPWL_ADDR = 6'h0) // servo pulse width low
  (input logic clk,
  input logic                   en1mhz, // clock enable at 1MHz rate
  input logic                   rstn,
  // Register access for registers in first 64
  input [5:0]                   adr,
  input [7:0]                   dbus_in,
  output [7:0]                  dbus_out,
  input                         iore,
  input                         iowe,
  output wire                   io_out_en,
  // Register access for registers not in first 64
  input wire [7:0]              ramadr,
  input wire                    ramre,
  input wire                    ramwe,
  input wire                    dm_sel,
  // External inputs/outputs
  output logic [NUM_SERVOS-1:0] servos_en,
  output logic [NUM_SERVOS-1:0] servos_out
  );

  /////////////////////////////////
  // Local Parameters
  /////////////////////////////////
  localparam NUM_TIMERS = (NUM_SERVOS <= 16) ? NUM_SERVOS : 16;
  // Registers in I/O address range x0-x3F (memory addresses -x20-0x5F)
  //  use the adr/iore/iowe inputs. Registers in the extended address
  //  range (memory address 0x60 and above) use ramadr/ramre/ramwe
  localparam  SVCR_DM_LOC   = (SVCR_ADDR >= 16'h60) ? 1 : 0;
  localparam  SVPWH_DM_LOC   = (SVPWH_ADDR >= 16'h60) ? 1 : 0;
  localparam  SVPWL_DM_LOC   = (SVPWL_ADDR >= 16'h60) ? 1 : 0;

  localparam SVEN_BIT   = 7;
  localparam SVDIS_BIT  = 6;
  localparam SVUP_BIT   = 5;
  localparam SVCHAN_LSB = 0;    

  /////////////////////////////////
  // Signals
  /////////////////////////////////
  /*AUTOREG*/
  /*AUTOWIRE*/ 
  logic svcr_sel;
  logic svpwh_sel;
  logic svpwl_sel;
  logic svcr_we ;
  logic svpwh_we ;
  logic svpwl_we ;
  logic svcr_re ;
  logic svpwh_re ;
  logic svpwl_re ;
  logic [7:0] svcr_rdata;
  logic       SVEN;
  logic [4:0] SVCHAN;
  logic [3:0] SVPWH;
  logic [7:0] SVPWL;
  logic [4:0] chan_in;
  logic [11:0] chan_pw [NUM_TIMERS-1:0]; // pulse width per channel
  logic [14:0]  timercnt; // Need to count to 20000us. That's 15 bits.

  /////////////////////////////////
  // Functions and Tasks
  /////////////////////////////////

  /////////////////////////////////
  // Main Code
  /////////////////////////////////

  assign svcr_sel = SVCR_DM_LOC ?  (dm_sel && ramadr == SVCR_ADDR ) : (adr[5:0] == SVCR_ADDR[5:0] ); 
  assign svpwh_sel = SVPWH_DM_LOC ?  (dm_sel && ramadr == SVPWH_ADDR ) : (adr[5:0] == SVPWH_ADDR[5:0] );
  assign svpwl_sel = SVPWL_DM_LOC ?  (dm_sel && ramadr == SVPWL_ADDR ) : (adr[5:0] == SVPWL_ADDR[5:0] );
  assign svcr_we = svcr_sel && (SVCR_DM_LOC ?  ramwe : iowe); 
  assign svpwh_we = svpwh_sel && (SVPWH_DM_LOC ?  ramwe : iowe);
  assign svpwl_we = svpwl_sel && (SVPWL_DM_LOC ?  ramwe : iowe); 
  assign svcr_re = svcr_sel && (SVCR_DM_LOC ?  ramre : iore); 
  assign svpwh_re = svpwh_sel && (SVPWH_DM_LOC ?  ramre : iore);
  assign svpwl_re = svpwl_sel && (SVPWL_DM_LOC ?  ramre : iore); 

  assign dbus_out =  ({8{svcr_sel}} & svcr_rdata) |
                     ({8{svpwh_sel}} & {4'h0,SVPWH}) | 
                     ({8{svpwl_sel}} & SVPWL); 
  assign io_out_en = svcr_re || 
                     svpwh_re ||
                     svpwl_re; 

   // Control Registers
  assign chan_in = dbus_in[SVCHAN_LSB +: 5];
  always @(posedge clk or negedge rstn) begin
    if (!rstn)  begin
      SVEN   <= 1'b0;
      SVCHAN <= 5'h0;
      servos_en <= {NUM_SERVOS{1'b0}};
    end else if (svcr_we) begin
      SVCHAN<= chan_in;
      SVEN  <= dbus_in[SVEN_BIT]  || 
                          (servos_en[chan_in] && ~dbus_in[SVDIS_BIT]);;
      servos_en[chan_in] <= dbus_in[SVEN_BIT] || 
                          (servos_en[chan_in] && ~dbus_in[SVDIS_BIT]);
    end else begin
      SVEN <= servos_en[SVCHAN];
    end
  end // always @ (posedge clk or negedge rstn)
  always @(posedge clk) begin
    if (svcr_we) begin
      // Max pulse we'll do is 2.4ms which is 2400 counts at 1MHz. Need 12 bits.
      if (dbus_in[SVUP_BIT]) chan_pw[chan_in[3:0]] <= {SVPWH,SVPWL};
    end
  end // always @ (posedge clk or negedge rstn)
  assign svcr_rdata = ({7'h0,SVEN}   << SVEN_BIT) |
                      ({3'h0,SVCHAN} << SVCHAN_LSB);
  always @(posedge clk or negedge rstn) begin
    if (!rstn)  begin
      SVPWH <= 4'h0;
    end else if (svpwh_we) begin
      SVPWH  <= dbus_in[3:0];
    end 
  end
  always @(posedge clk or negedge rstn) begin
    if (!rstn)  begin
      SVPWL <= 8'h0;
    end else if (svpwl_we) begin
      SVPWL  <= dbus_in;
    end
  end

  // Run the counter at 1MHz (divby16) for 1us resolution
  always @(posedge clk or negedge rstn) begin
    if (!rstn) begin
      /*AUTORESET*/
      // Beginning of autoreset for uninitialized flops
      timercnt <= 15'h0;
      // End of automatics
    end else if (en1mhz && |servos_en) begin
      // it takes 20000 cycles of 1MHz to get 20ms
      timercnt <= (timercnt == 15'd19999) ? 15'd0 : (timercnt + 15'd1);
    end
  end // always @ (posedge clk or negedge rstn)
  // Save gates by using common timer for all channels
  // By modifying 2 of the top 3 bits of the count, we can
  //  easily get up to four channels non-overlapping. Getting
  //  more to be non-overlapping is probably not worth the
  //  extra logic it would require.
  genvar i;
  generate 
    for (i=0;i<NUM_SERVOS;i++) begin : gen_chan
      always @(posedge clk or negedge rstn) begin
        if (!rstn) begin
          servos_out[i] <= 1'b0;
        end else begin
          // Do full less-than check instead of just checking for equals at the ends to 
          //  avoid missing a change in cases where chan_pw changes
          servos_out[i] <= servos_en[i] && (timercnt[14:12] == {1'b0,i[1:0]}) && (timercnt[11:0] <= chan_pw[i[3:0]]);
        end
      end // always @ (posedge clk or negedge rstn)
    end // block: gen_chan
  endgenerate
  
   /////////////////////////////////
   // Assertions
   /////////////////////////////////

`ifdef STGI_ASSERT_ON
  assert property (@(posedge clk) disable iff (~rstn) (timercnt<=20000))
               else $display("ERROR: xlr8_servo : counter out of range");
`endif

   /////////////////////////////////
   // Cover Points
   /////////////////////////////////

`ifdef SUP_COVER_ON
`endif

endmodule

