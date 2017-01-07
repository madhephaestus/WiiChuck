//=================================================================
//  Copyright(c) Alorium Technology Group Inc., 2016
//  ALL RIGHTS RESERVED
//  $Id:  $
//=================================================================
//
// File name:  : xlr8_top.v
// Author      : Matt Weber
// Contact     : matt@aloriumtech.com linkedin.com/in/mattweberdesign
// Description : OpenXLR8 top module with XLR8Servo library included
//
//=================================================================
///////////////////////////////////////////////////////////////////


module xlr8_top
  (
   //Clock and Reset
   input        Clock, // 16MHz
   input        RESET_N,
   //Arduino I/Os
   inout        SCL,
   inout        SDA,
   // The D13..D2,TX,RX go through level shift on board before getting to pins
   inout        D13,D12,D11,D10,D9,D8, // Port B
   inout        D7,D6,D5,D4,D3,D2,TX,RX, // Port D
   // A5..A0 are labeled DIG_IO_5-0 on schematic
   inout        A5,A4,A3,A2,A1,A0, // Some stuff on board between here and the actual header pins
   output       PIN13LED,
   // We can disconnect Ana_Dig from ADC inputs if necessary (don't know if it is) by driving
   //   OE low. Else leave OE as high-Z (don't drive it high).
   inout [5:0]  DIG_IO_OE,
   output       ANA_UP, // Choose ADC ref between 1=AREF pin and 0=regulated 3.3V
   output       I2C_ENABLE, // 0=disable pullups on sda/scl, 1=enable pullups
   // JTAG connector reused as digial IO. On that connector, pin 4 is power, pins 2&10 are ground
   //   and pin 8 selects between gpio (low) and jtag (high) modes and has a pulldown.
   inout        JT9, // external pullup. JTAG function is TDI
   inout        JT7, // no JTAG function
   inout        JT6, // no JTAG function
   inout        JT5, // external pullup. JTAG function is TMS
   inout        JT3, // JTAG function TDO
   inout        JT1, // external pulldown, JTAG function is TCK
   // Interface to EEPROM or other device in SOIC-8 spot on the board
   inout        SOIC7, // WP in the case of an 24AA128SM EEPROM
   inout        SOIC6, // SCL in the case of an 24AA128SM EEPROM
   inout        SOIC5, // SDA in the case of an 24AA128SM EEPROM
   inout        SOIC3, // A2 in the case of an 24AA128SM EEPROM
   inout        SOIC2, // A1 in the case of an 24AA128SM EEPROM
   inout        SOIC1  // A0 in the case of an 24AA128SM EEPROM
   );

  localparam dm_size       = 2;  //  2KB Data memory to match ATmega328p
  localparam pm_size       = 16; // 16KInstructions to match ATmega328p
  localparam UFM_ADR_WIDTH = 13; // (For 32KB pmem = 16KInstructions = 8K 32-bit accesses = 13 bit address)
  localparam NUM_PINS     = 20; // A[5:0] and D[13:0]
  localparam DESIGN_CONFIG = 8; // 8=16MHz, 10=32MHZ, 12=64MHz (future)
  localparam CLOCK_SELECT = DESIGN_CONFIG[2:1]; // 2 bits. 0=16MHZ, 1=32MHz, 2=64MHz, 3=reserved
  localparam PLL_SELECT   = DESIGN_CONFIG[4];  // 1=50MHz PLL, 0=16MHz PLL
  localparam NUM_XBS      = 2;        // 1=All the built-in XBs (ADC, internal OSC, etc) but nothing else
  localparam APP_XB0_ENABLE = 32'h2;  // ID indicates which XBs beyond the built-in ones are included, [7:0] are reserved
                                      //   typically use bit [0] for floating point, [1] for servo, [2] for neopixel
  localparam NUM_SERVOS = NUM_PINS; // Allow servo signal on any pin 
    
`include "avr_adr_pack.vh"
`include "xb_adr_pack.vh"


  /*AUTOWIRE*/
  // Beginning of automatic wires (for undeclared instantiated-module outputs)
  wire [5:0]		ADCD;			// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire			ICP1_pin;		// From iomux328_inst of xlr8_iomux328.v
  wire			INT0_rcv;		// From iomux328_inst of xlr8_iomux328.v
  wire			INT1_rcv;		// From iomux328_inst of xlr8_iomux328.v
  wire			OC0A_enable;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire			OC0A_pin;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire			OC0B_enable;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire			OC0B_pin;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire			OC1A_enable;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire			OC1A_pin;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire			OC1B_enable;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire			OC1B_pin;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire			OC2A_enable;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire			OC2A_pin;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire			OC2B_enable;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire			OC2B_pin;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire			RXD_rcv;		// From iomux328_inst of xlr8_iomux328.v
  wire			T0_pin;			// From iomux328_inst of xlr8_iomux328.v
  wire			T1_pin;			// From iomux328_inst of xlr8_iomux328.v
  wire			TXD;			// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  logic			clk_adcref;		// From clocks_inst of xlr8_clocks.v
  logic			clk_cpu;		// From clocks_inst of xlr8_clocks.v
  logic			clk_io;			// From clocks_inst of xlr8_clocks.v
  wire [7:0]		core_ramadr_lo8;	// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire			core_rstn;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  logic [23:0]		debug_bus;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire [15:0]		dm_adr;			// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire			dm_ce;			// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire [7:0]		dm_dout;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire			dm_we;			// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire [1:0]		eimsk;			// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  logic			en128khz;		// From clocks_inst of xlr8_clocks.v
  logic			en16mhz;		// From clocks_inst of xlr8_clocks.v
  logic			en1mhz;			// From clocks_inst of xlr8_clocks.v
  wire [255:0]		gprf;			// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  logic			intosc_div1024;		// From clocks_inst of xlr8_clocks.v
  logic			intosc_div1024_en;	// From gpio_inst of xlr8_gpio.v
  logic			locked_adcref;		// From clocks_inst of xlr8_clocks.v
  wire			misoi;			// From iomux328_inst of xlr8_iomux328.v
  wire			misoo;			// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire			mosii;			// From iomux328_inst of xlr8_iomux328.v
  wire			mosio;			// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire [7:0]		msts_dbusout;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire [2:0]		pcie;			// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire [23:0]		pcint_rcv;		// From iomux328_inst of xlr8_iomux328.v
  wire [23:0]		pcmsk;			// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  logic [15:0]		pm_addr;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  logic			pm_ce;			// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  logic [15:0]		pm_core_rd_addr;	// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  logic [15:0]		pm_core_rd_data;	// From p_mem_inst of xlr8_p_mem.v
  logic [15:0]		pm_rd_data;		// From p_mem_inst of xlr8_p_mem.v
  logic			pm_wr;			// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  logic [15:0]		pm_wr_data;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire [5:0]		portb_ddrx;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire [5:0]		portb_pinx;		// From iomux328_inst of xlr8_iomux328.v
  wire [5:0]		portb_portx;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire [5:0]		portc_ddrx;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire [5:0]		portc_pinx;		// From iomux328_inst of xlr8_iomux328.v
  wire [5:0]		portc_portx;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire [7:0]		portd_ddrx;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire [7:0]		portd_pinx;		// From iomux328_inst of xlr8_iomux328.v
  wire [7:0]		portd_portx;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  logic			pwr_on_nrst;		// From clocks_inst of xlr8_clocks.v
  logic			rst_flash_n;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire			scki;			// From iomux328_inst of xlr8_iomux328.v
  wire			scko;			// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  logic [NUM_SERVOS-1:0] servos_en;		// From servo_inst of xlr8_servo.v
  logic [NUM_SERVOS-1:0] servos_out;		// From servo_inst of xlr8_servo.v
  wire			spe;			// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire			spimaster;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire			ss_b;			// From iomux328_inst of xlr8_iomux328.v
  wire [7:0]		stgi_xf_servo_dbusout;	// From servo_inst of xlr8_servo.v
  wire			stgi_xf_servo_out_en;	// From servo_inst of xlr8_servo.v
  wire			twen;			// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire			uart_rx_en;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  wire			uart_tx_en;		// From uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  logic [NUM_PINS-1:0]	xb_ddoe;		// From xb_pinmux_inst of xlr8_xb_pinmux.v
  logic [NUM_PINS-1:0]	xb_ddov;		// From xb_pinmux_inst of xlr8_xb_pinmux.v
  logic [NUM_PINS-1:0]	xb_pvoe;		// From xb_pinmux_inst of xlr8_xb_pinmux.v
  logic [NUM_PINS-1:0]	xb_pvov;		// From xb_pinmux_inst of xlr8_xb_pinmux.v
  wire			xck_rcv;		// From iomux328_inst of xlr8_iomux328.v
  wire [7:0]		xlr8_clocks_dbusout;	// From clocks_inst of xlr8_clocks.v
  wire			xlr8_clocks_out_en;	// From clocks_inst of xlr8_clocks.v
  logic [7:0]		xlr8_gpio_dbusout;	// From gpio_inst of xlr8_gpio.v
  logic			xlr8_gpio_out_en;	// From gpio_inst of xlr8_gpio.v
  // End of automatics
  /*AUTOREGINPUT*/
  // Beginning of automatic reg inputs (for undeclared instantiated-module inputs)
  logic [23:0]		xb_info;		// To uc_top_wrp_vlog_inst of xlr8_atmega328clone.v
  // End of automatics

   wire [23:0]          pcint_irq;
   wire                 PUD, SLEEP;
  wire[15:0]              pm_din;
  wire[7:0]               dm_din;
  reg                     reset_n_r;
  reg                     reset_n_rr;
  wire                  OC0_PWM0;
  wire                  OC1A_PWM1A;
  wire                  OC1B_PWM1B;
  wire                  OC2_PWM2;
  wire [7:0]           interrupts;
  wire                 sdain,sclin,msdain,msclin;
  wire                 sdaout,sclout,msdaout,msclout;
  wire                 sdaen,sclen,msdaen,msclen;
   //FP interface
   wire [5:0]          io_arb_mux_adr;
   wire                io_arb_mux_iore;
   wire                io_arb_mux_iowe;
   wire [7:0]          io_arb_mux_dbusout;
   wire [7:0]          stgi_xf_io_slv_dbusout;
   wire                stgi_xf_io_slv_out_en;
   wire [7:0]          stgi_xf_float_dbusout;
   wire                stgi_xf_float_out_en;
   wire [7:0]          core_ramadr;
   wire                core_ramre;
   wire                core_ramwe;
   wire                core_dm_sel;
   // connections for stgi_xf ports
   wire [31:0]         xf_dataa;
   wire [31:0]         xf_datab;
   wire [7:0]          xf_en;
   wire [31:0]         xf_p0_result;
   wire [31:0]         xf_p1_result;
   wire [31:0]         xf_p2_result;
   wire [31:0]         xf_p3_result;
   wire [31:0]         xf_p4_result;
   wire [31:0]         xf_p5_result;
   wire [31:0]         xf_p6_result;
   wire [31:0]         xf_p7_result;
   logic [NUM_XBS-1:0][NUM_PINS-1:0] xbs_ddoe; // override data direction
   logic [NUM_XBS-1:0][NUM_PINS-1:0] xbs_ddov; // data direction value if overridden (1=out)
   logic [NUM_XBS-1:0][NUM_PINS-1:0] xbs_pvoe; // override output value
   logic [NUM_XBS-1:0][NUM_PINS-1:0] xbs_pvov; // output value if overridden

  /* xlr8_clocks AUTO_TEMPLATE
               (// Register read/write
                .adr                   (io_arb_mux_adr[]),
                .dbus_in               (io_arb_mux_dbusout[]),
                .dbus_out              (xlr8_clocks_dbusout[]),
                .iore                  (io_arb_mux_iore[]),
                .iowe                  (io_arb_mux_iowe[]),
                .io_out_en             (xlr8_clocks_out_en[]),
                .ramadr                (core_ramadr_lo8[]),
                .ramre                 (core_ramre[]),
                .ramwe                 (core_ramwe[]),
                .dm_sel                (core_dm_sel[]),
               );
   */
  xlr8_clocks #(.CLOCK_SELECT           (CLOCK_SELECT),
                .PRR_ADDR               (PRR_ADDR),
                .PLL_SELECT             (PLL_SELECT))
  clocks_inst (// Not using these clock domains yet
               // Outputs
               .clk_io                  (clk_io),
               .clk_usart0              (),
               .clk_spi                 (),
               .clk_tim1                (),
               .clk_intosc              (),
               .clk_tim0                (),
               .clk_tim2                (),
               .clk_twi                 (),
               /*AUTOINST*/
	       // Outputs
	       .pwr_on_nrst		(pwr_on_nrst),
	       .dbus_out		(xlr8_clocks_dbusout[7:0]), // Templated
	       .io_out_en		(xlr8_clocks_out_en),	 // Templated
	       .locked_adcref		(locked_adcref),
	       .clk_cpu			(clk_cpu),
	       .clk_adcref		(clk_adcref),
	       .intosc_div1024		(intosc_div1024),
	       .en16mhz			(en16mhz),
	       .en1mhz			(en1mhz),
	       .en128khz		(en128khz),
	       // Inputs
	       .Clock			(Clock),
	       .core_rstn		(core_rstn),
	       .adr			(io_arb_mux_adr[5:0]),	 // Templated
	       .dbus_in			(io_arb_mux_dbusout[7:0]), // Templated
	       .iore			(io_arb_mux_iore),	 // Templated
	       .iowe			(io_arb_mux_iowe),	 // Templated
	       .ramadr			(core_ramadr_lo8[7:0]),	 // Templated
	       .ramre			(core_ramre),		 // Templated
	       .ramwe			(core_ramwe),		 // Templated
	       .dm_sel			(core_dm_sel));		 // Templated

        // IOs
        // On this board, the analog pins are inputs only and go right to the
        //   ADC; They don't have the other functions that ATMega328 has muxed to them
   assign PUD = 1'b0; // FIXME: should come from MCUCR register
   assign SLEEP = 1'b0; // FIXME: Add this
   wire                pcie0,pcie1,pcie2,UMSEL,xcko,INT1_enable,INT0_enable;
   assign {UMSEL,xcko} = 2'h0;
   //ext and pinchange interrupt enables and masks (sourced from stgi_ext_int in uc_top)
   assign {pcie0,pcie1,pcie2} = pcie;
   assign {INT1_enable,INT0_enable} = eimsk;

   // Mux Xcelerator Block control of pins and send result into AVR io mux
   assign xbs_ddoe[0] = intosc_div1024_en << 8; // put intosc on PB0 so ICP function can measure it accurately
   assign xbs_ddov[0] = {NUM_PINS{1'b1}}; // force to be outputs
   assign xbs_pvoe[0] = xbs_ddoe[0];
   assign xbs_pvov[0] = intosc_div1024 << 8; // put intosc on PB0 so ICP function can measure it accurately
   assign xbs_ddoe[1] = {NUM_PINS{1'b0}}; // Library still sets the data direction
   assign xbs_ddov[1] = {NUM_PINS{1'b0}};
   assign xbs_pvoe[1] = servos_en[19:0];
   assign xbs_pvov[1] = servos_out[19:0];

   xlr8_xb_pinmux #(.NUM_PINS           (NUM_PINS), // Digital 0-13 and AnaDig 0-5
                    .NUM_XBS            (NUM_XBS))
        xb_pinmux_inst (.clk            (Clock), // clock is just used for assertions
                        .rstn           (rst_flash_n), // reset is just for assertions, logic works all the time
                        /*AUTOINST*/
			// Outputs
			.xb_ddoe	(xb_ddoe[NUM_PINS-1:0]),
			.xb_ddov	(xb_ddov[NUM_PINS-1:0]),
			.xb_pvoe	(xb_pvoe[NUM_PINS-1:0]),
			.xb_pvov	(xb_pvov[NUM_PINS-1:0]),
			// Inputs
			.xbs_ddoe	(xbs_ddoe/*[NUM_XBS-1:0][NUM_PINS-1:0]*/),
			.xbs_ddov	(xbs_ddov/*[NUM_XBS-1:0][NUM_PINS-1:0]*/),
			.xbs_pvoe	(xbs_pvoe/*[NUM_XBS-1:0][NUM_PINS-1:0]*/),
			.xbs_pvov	(xbs_pvov/*[NUM_XBS-1:0][NUM_PINS-1:0]*/));

  /* xlr8_iomux328 AUTO_TEMPLATE (
     .pcint_irq (pcmsk[23:0]),
     );*/

  // Currently do not have pullups on A0-A5, so no need to disconnect
  //  them when doing ADC read
  assign DIG_IO_OE = 6'hZ; // 0=disconnected, high-Z=connected
  //assign DIG_IO_OE[0] = adc_active[0] ? 1'b0 : 1'bz;
  //assign DIG_IO_OE[1] = adc_active[1] ? 1'b0 : 1'bz;
  //assign DIG_IO_OE[2] = adc_active[2] ? 1'b0 : 1'bz;
  //assign DIG_IO_OE[3] = adc_active[3] ? 1'b0 : 1'bz;
  //assign DIG_IO_OE[4] = adc_active[4] ? 1'b0 : 1'bz;
  //assign DIG_IO_OE[5] = adc_active[5] ? 1'b0 : 1'bz;
  
  // Temporarily just send some debug signals out
  assign JT9    = RESET_N;
  assign JT7    = core_rstn;
  assign JT6    = locked_adcref;
  assign JT5    = pwr_on_nrst;
  assign JT3    = rst_flash_n;
  assign JT1    = clk_cpu;
  // FIXME: these aren't hooked up yet
  assign SOIC7  = 1'bZ;
  assign SOIC6  = 1'bZ;
  assign SOIC5  = 1'bZ;
  assign SOIC3  = 1'bZ;
  assign SOIC2  = 1'bZ;
  assign SOIC1  = 1'bZ;
   xlr8_iomux328 iomux328_inst (// Inouts
                                .portb_pads     ({D13,D12,D11,D10,D9,D8}),
                                .portc_pads     ({A5,A4,A3,A2,A1,A0}),
                                .portd_pads     ({D7,D6,D5,D4,D3,D2,TX,RX}),
                                .clk            (clk_cpu), // run this regardless of any sleep
                                // Inouts
                                .SDA            (SDA),
                                .SCL            (SCL),
                                /*AUTOINST*/
				// Outputs
				.portb_pinx	(portb_pinx[5:0]),
				.portc_pinx	(portc_pinx[5:0]),
				.portd_pinx	(portd_pinx[7:0]),
				.pcint_rcv	(pcint_rcv[23:0]),
				.scki		(scki),
				.misoi		(misoi),
				.mosii		(mosii),
				.ss_b		(ss_b),
				.ICP1_pin	(ICP1_pin),
				.PIN13LED	(PIN13LED),
				.sdain		(sdain),
				.sclin		(sclin),
				.I2C_ENABLE	(I2C_ENABLE),
				.xck_rcv	(xck_rcv),
				.T1_pin		(T1_pin),
				.T0_pin		(T0_pin),
				.INT1_rcv	(INT1_rcv),
				.INT0_rcv	(INT0_rcv),
				.RXD_rcv	(RXD_rcv),
				// Inputs
				.core_rstn	(core_rstn),
				.PUD		(PUD),
				.SLEEP		(SLEEP),
				.ADCD		(ADCD[5:0]),
				.portb_portx	(portb_portx[5:0]),
				.portb_ddrx	(portb_ddrx[5:0]),
				.portc_portx	(portc_portx[5:0]),
				.portc_ddrx	(portc_ddrx[5:0]),
				.portd_portx	(portd_portx[7:0]),
				.portd_ddrx	(portd_ddrx[7:0]),
				.xb_ddoe	(xb_ddoe[19:0]),
				.xb_ddov	(xb_ddov[19:0]),
				.xb_pvoe	(xb_pvoe[19:0]),
				.xb_pvov	(xb_pvov[19:0]),
				.pcint_irq	(pcmsk[23:0]),	 // Templated
				.pcie0		(pcie0),
				.pcie1		(pcie1),
				.pcie2		(pcie2),
				.spe		(spe),
				.spimaster	(spimaster),
				.scko		(scko),
				.misoo		(misoo),
				.mosio		(mosio),
				.OC2A_enable	(OC2A_enable),
				.OC2A_pin	(OC2A_pin),
				.OC1B_enable	(OC1B_enable),
				.OC1B_pin	(OC1B_pin),
				.OC1A_enable	(OC1A_enable),
				.OC1A_pin	(OC1A_pin),
				.twen		(twen),
				.sdaout		(sdaout),
				.sclout		(sclout),
				.UMSEL		(UMSEL),
				.xcko		(xcko),
				.OC2B_enable	(OC2B_enable),
				.OC2B_pin	(OC2B_pin),
				.OC0B_enable	(OC0B_enable),
				.OC0B_pin	(OC0B_pin),
				.OC0A_enable	(OC0A_enable),
				.OC0A_pin	(OC0A_pin),
				.INT1_enable	(INT1_enable),
				.INT0_enable	(INT0_enable),
				.uart_tx_en	(uart_tx_en),
				.TXD		(TXD),
				.uart_rx_en	(uart_rx_en));


        // Data Memory (SRAM)
xlr8_d_mem #(.dm_size(dm_size))
d_mem_inst(
   .cp2     (clk_cpu),
   .ce      (dm_ce),
   .address (dm_adr),
   .din     (dm_dout),
   .dout    (dm_din),
   .we      (dm_we)
);


  //////////////////////////////////////////////////////////////////
  // program memory
  //////////////////////////////////////////////////////////////////

  /* xlr8_pmem AUTO_TEMPLATE(
   .\(we\)\(.*\) (pm_\1_\2),
   );*/
xlr8_p_mem #(.pm_size(pm_size))
p_mem_inst
  (
   .clk     (clk_cpu),
   /*AUTOINST*/
   // Outputs
   .pm_core_rd_data			(pm_core_rd_data[15:0]),
   .pm_rd_data				(pm_rd_data[15:0]),
   // Inputs
   .rst_flash_n				(rst_flash_n),
   .pm_core_rd_addr			(pm_core_rd_addr[15:0]),
   .pm_ce				(pm_ce),
   .pm_wr				(pm_wr),
   .pm_wr_data				(pm_wr_data[15:0]),
   .pm_addr				(pm_addr[15:0]));


        // AVR core
xlr8_atmega328clone
                #(.UFM_BC_WIDTH         (4),
                  .PM_REAL_SIZE         (pm_size),
                  /*AUTOINSTPARAM*/
		  // Parameters
		  .DESIGN_CONFIG	(DESIGN_CONFIG),
		  .UFM_ADR_WIDTH	(UFM_ADR_WIDTH))

     uc_top_wrp_vlog_inst(
                         .nrst        (RESET_N),
                         .clk         (clk_cpu ),
                         .pcint_rcv   (pcint_rcv[23:0]),
                          // UART related
                         .rxd         (RXD_rcv),
			  // Outputs
                         .txd         (TXD),
                          /*AUTOINST*/
			  // Outputs
			  .core_rstn		(core_rstn),
			  .rst_flash_n		(rst_flash_n),
			  .portb_portx		(portb_portx[5:0]),
			  .portb_ddrx		(portb_ddrx[5:0]),
			  .portc_portx		(portc_portx[5:0]),
			  .portc_ddrx		(portc_ddrx[5:0]),
			  .portd_portx		(portd_portx[7:0]),
			  .portd_ddrx		(portd_ddrx[7:0]),
			  .ADCD			(ADCD[5:0]),
			  .ANA_UP		(ANA_UP),
			  .OC0A_pin		(OC0A_pin),
			  .OC0B_pin		(OC0B_pin),
			  .OC1A_pin		(OC1A_pin),
			  .OC1B_pin		(OC1B_pin),
			  .OC2A_pin		(OC2A_pin),
			  .OC2B_pin		(OC2B_pin),
			  .OC0A_enable		(OC0A_enable),
			  .OC0B_enable		(OC0B_enable),
			  .OC1A_enable		(OC1A_enable),
			  .OC1B_enable		(OC1B_enable),
			  .OC2A_enable		(OC2A_enable),
			  .OC2B_enable		(OC2B_enable),
			  .uart_rx_en		(uart_rx_en),
			  .uart_tx_en		(uart_tx_en),
			  .misoo		(misoo),
			  .mosio		(mosio),
			  .scko			(scko),
			  .spe			(spe),
			  .spimaster		(spimaster),
			  .twen			(twen),
			  .sdaout		(sdaout),
			  .sclout		(sclout),
			  .pcmsk		(pcmsk[23:0]),
			  .pcie			(pcie[2:0]),
			  .eimsk		(eimsk[1:0]),
			  .pm_ce		(pm_ce),
			  .pm_wr		(pm_wr),
			  .pm_wr_data		(pm_wr_data[15:0]),
			  .pm_addr		(pm_addr[15:0]),
			  .pm_core_rd_addr	(pm_core_rd_addr[15:0]),
			  .dm_adr		(dm_adr[15:0]),
			  .dm_dout		(dm_dout[7:0]),
			  .dm_ce		(dm_ce),
			  .dm_we		(dm_we),
			  .core_ramadr_lo8	(core_ramadr_lo8[7:0]),
			  .core_ramre		(core_ramre),
			  .core_ramwe		(core_ramwe),
			  .core_dm_sel		(core_dm_sel),
			  .io_arb_mux_adr	(io_arb_mux_adr[5:0]),
			  .io_arb_mux_iore	(io_arb_mux_iore),
			  .io_arb_mux_iowe	(io_arb_mux_iowe),
			  .io_arb_mux_dbusout	(io_arb_mux_dbusout[7:0]),
			  .msts_dbusout		(msts_dbusout[7:0]),
			  .gprf			(gprf[8*32-1:0]),
			  .debug_bus		(debug_bus[23:0]),
			  // Inputs
			  .en16mhz		(en16mhz),
			  .en128khz		(en128khz),
			  .clk_adcref		(clk_adcref),
			  .locked_adcref	(locked_adcref),
			  .pwr_on_nrst		(pwr_on_nrst),
			  .portb_pinx		(portb_pinx[5:0]),
			  .portc_pinx		(portc_pinx[5:0]),
			  .portd_pinx		(portd_pinx[7:0]),
			  .T0_pin		(T0_pin),
			  .T1_pin		(T1_pin),
			  .ICP1_pin		(ICP1_pin),
			  .misoi		(misoi),
			  .mosii		(mosii),
			  .scki			(scki),
			  .ss_b			(ss_b),
			  .sdain		(sdain),
			  .sclin		(sclin),
			  .pm_rd_data		(pm_rd_data[15:0]),
			  .pm_core_rd_data	(pm_core_rd_data[15:0]),
			  .dm_din		(dm_din[7:0]),
			  .stgi_xf_io_slv_dbusout(stgi_xf_io_slv_dbusout[7:0]),
			  .stgi_xf_io_slv_out_en(stgi_xf_io_slv_out_en),
			  .xb_info		(xb_info[23:0]));

// GPIO module has the AVR gpio registers as well as a few optional
  //  read only registers that could be used to pass configuration
  //  information to the software. Setting address to zero eliminates
  //  the register
  /* xlr8_gpio AUTO_TEMPLATE
               (.clk                   (clk_cpu),
                .clken                 (1'b1),
                .rstn                  (core_rstn),
                // Register read/write
                .adr                   (io_arb_mux_adr[]),
                .dbus_in               (io_arb_mux_dbusout[]),
                .dbus_out              (xlr8_gpio_dbusout[]),
                .iore                  (io_arb_mux_iore[]),
                .iowe                  (io_arb_mux_iowe[]),
                .io_out_en             (xlr8_gpio_out_en[]),
                .ramadr                (core_ramadr_lo8[]),
                .ramre                 (core_ramre[]),
                .ramwe                 (core_ramwe[]),
                .dm_sel                (core_dm_sel[]),
               );
   */
 xlr8_gpio #(.GPIOR0_ADDR             (GPIOR0_Address),
             .GPIOR1_ADDR             (GPIOR1_Address),
             .GPIOR2_ADDR             (GPIOR2_Address),
             .READREG0_ADDR           (6'h0),
             .READREG1_ADDR           (6'h0),
             .READREG2_ADDR           (6'h0),
             .READREG3_ADDR           (6'h0),
             .READREG0_VAL            (8'h0),
             .READREG1_VAL            (8'h0),
             .READREG2_VAL            (8'h0),
             .READREG3_VAL            (8'h0),
             /*AUTOINSTPARAM*/
	     // Parameters
	     .DESIGN_CONFIG		(DESIGN_CONFIG),
	     .APP_XB0_ENABLE		(APP_XB0_ENABLE),
	     .CLKSPD_ADDR		(CLKSPD_ADDR))
  gpio_inst (/*AUTOINST*/
	     // Outputs
	     .dbus_out			(xlr8_gpio_dbusout[7:0]), // Templated
	     .io_out_en			(xlr8_gpio_out_en),	 // Templated
	     .intosc_div1024_en		(intosc_div1024_en),
	     // Inputs
	     .clk			(clk_cpu),		 // Templated
	     .rstn			(core_rstn),		 // Templated
	     .clken			(1'b1),			 // Templated
	     .adr			(io_arb_mux_adr[5:0]),	 // Templated
	     .dbus_in			(io_arb_mux_dbusout[7:0]), // Templated
	     .iore			(io_arb_mux_iore),	 // Templated
	     .iowe			(io_arb_mux_iowe),	 // Templated
	     .ramadr			(core_ramadr_lo8[7:0]),	 // Templated
	     .ramre			(core_ramre),		 // Templated
	     .ramwe			(core_ramwe),		 // Templated
	     .dm_sel			(core_dm_sel));		 // Templated

   assign stgi_xf_io_slv_dbusout = stgi_xf_servo_out_en    ? stgi_xf_servo_dbusout :
                                   xlr8_clocks_out_en      ? xlr8_clocks_dbusout :
                                                             xlr8_gpio_dbusout;
   assign stgi_xf_io_slv_out_en  = stgi_xf_servo_out_en ||
                                   xlr8_clocks_out_en || xlr8_gpio_out_en;
 

  /* xlr8_servo AUTO_TEMPLATE
               (.clk                   (clk_cpu),
                .rstn                  (core_rstn),
                // Register read/write
                .adr                   (io_arb_mux_adr[]),
                .dbus_in               (io_arb_mux_dbusout[]),
                .dbus_out              (stgi_xf_servo_dbusout[]),
                .iore                  (io_arb_mux_iore[]),
                .iowe                  (io_arb_mux_iowe[]),
                .io_out_en             (stgi_xf_servo_out_en[]),
                .ramadr                (core_ramadr_lo8[]),
                .ramre                 (core_ramre[]),
                .ramwe                 (core_ramwe[]),
                .dm_sel                (core_dm_sel[]),
               );
   */
   xlr8_servo #(.NUM_SERVOS             (NUM_SERVOS),
                .SVCR_ADDR              (SVCR_ADDR ),
                .SVPWL_ADDR             (SVPWL_ADDR),
                .SVPWH_ADDR             (SVPWH_ADDR))
     servo_inst (/*AUTOINST*/
		 // Outputs
		 .dbus_out		(stgi_xf_servo_dbusout[7:0]), // Templated
		 .io_out_en		(stgi_xf_servo_out_en),	 // Templated
		 .servos_en		(servos_en[NUM_SERVOS-1:0]),
		 .servos_out		(servos_out[NUM_SERVOS-1:0]),
		 // Inputs
		 .clk			(clk_cpu),		 // Templated
		 .en1mhz		(en1mhz),
		 .rstn			(core_rstn),		 // Templated
		 .adr			(io_arb_mux_adr[5:0]),	 // Templated
		 .dbus_in		(io_arb_mux_dbusout[7:0]), // Templated
		 .iore			(io_arb_mux_iore),	 // Templated
		 .iowe			(io_arb_mux_iowe),	 // Templated
		 .ramadr		(core_ramadr_lo8[7:0]),	 // Templated
		 .ramre			(core_ramre),		 // Templated
		 .ramwe			(core_ramwe),		 // Templated
		 .dm_sel		(core_dm_sel));		 // Templated
  

  endmodule

// Local Variables:
// verilog-library-directories:("." "../../../XLR8Core/extras/rtl/" "../../../XLR8Servo/extras/rtl/")
// verilog-library-files:("../../../XLR8Core/extras/rtl/xlr8_atmega328clone_bb.v")
// eval:(verilog-read-defines)
// End:
