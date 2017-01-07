#define FXOS8700_I2C_ADDR0           0x1E // SA1 = Gnd, SA0 = Gnd
#define FXOS8700_I2C_ADDR1           0x1D // SA1 = Gnd, SA0 = Vcc
#define FXOS8700_I2C_ADDR2           0x1C // SA1 = Vcc, SA0 = Gnd
#define FXOS8700_I2C_ADDR3           0x1F // SA1 = Vcc, SA0 = Vcc
#define FXOS8700_STATUS              0x00 // Real-time data-ready status or FIFO status
#define FXOS8700_OUT_X_MSB           0x01 // 8 MSBs of 14-bit sample / Root pointer to XYZ FIFO
#define FXOS8700_OUT_X_LSB           0x02 // 6 LSBs of 14-bit real-time sample
#define FXOS8700_OUT_Y_MSB           0x03 // 8 MSBs of 14-bit real-time sample
#define FXOS8700_OUT_Y_LSB           0x04 // 6 LSBs of 14-bit real-time sample
#define FXOS8700_OUT_Z_MSB           0x05 // 8 MSBs of 14-bit real-time sample
#define FXOS8700_OUT_Z_LSB           0x06 // 6 LSBs of 14-bit real-time sample
#define FXOS8700_F_SETUP             0x09 // FIFO setup
#define FXOS8700_TRIG_CFG            0x0A // FIFO event trigger configuration register
#define FXOS8700_SYSMOD              0x0B // Current system mode
#define FXOS8700_INT_SOURCE          0x0C // Interrupt status
#define FXOS8700_WHO_AM_I            0x0D // Device ID
#define FXOS8700_XYZ_DATA_CFG        0x0E // Acceleration dynamic range and filter enable settings
#define FXOS8700_P_FILTER_CUTOFF     0x0F // Pulse detection high- pass and low-pass filter enable bits. High-pass filter cutoff frequency selection
#define FXOS8700_PL_STATUS           0x10 // Landscape/Portrait orientation status
#define FXOS8700_PL_CFG              0x11 // Landscape/Portrait configuration
#define FXOS8700_PL_COUNT            0x12 // Landscape/Portrait debounce counter
#define FXOS8700_PL_BF_ZCOMP         0x13 // Back/Front Trip angle threshold
#define FXOS8700_PL_THS_REG          0x14 // Portrait to Landscape Trip Threshold angle and hysteresis settings
#define FXOS8700_A_FFMT_CFG          0x15 // Freefall/Motion function configuration
#define FXOS8700_A_FFMT_SRC          0x16 // Freefall/Motion event source register
#define FXOS8700_A_FFMT_THS          0x17 // Freefall/Motion threshold register
#define FXOS8700_A_FFMT_COUNT        0x18 // Freefall/Motion debounce counter
#define FXOS8700_TRANSIENT_CFG       0x1D // FIFO setup
#define FXOS8700_TRANSIENT_SRC       0x1E // Transient event status register
#define FXOS8700_TRANSIENT_THS       0x1F // Transient event threshold
#define FXOS8700_TRANSIENT_COUNT     0x20 // Transient debounce counter
#define FXOS8700_PULSE_CFG           0x21 // Pulse function configuration
#define FXOS8700_PULSE_SRC           0x22 // Pulse function source register
#define FXOS8700_PULSE_THSX          0x23 // X-axis pulse threshold
#define FXOS8700_PULSE_THSY          0x24 // Y-axis pulse threshold
#define FXOS8700_PULSE_THSZ          0x25 // Z-axis pulse threshold
#define FXOS8700_PULSE_TMLT          0x26 // Time limit for pulse detection
#define FXOS8700_PULSE_LTCY          0x27 // Latency time for second pulse detection
#define FXOS8700_PULSE_WIND          0x28 // Window time for second pulse detection
#define FXOS8700_ASLP_COUNT          0x29 // In activity counter setting for Auto-Sleep
#define FXOS8700_CTRL_REG1           0x2A // System ODR, accelerometer OSR, operating mode
#define FXOS8700_CTRL_REG2           0x2B // Self-Test, Reset, accelerometer OSR and Sleep mode settings
#define FXOS8700_CTRL_REG3           0x2C // Sleep mode interrupt wake enable, interrupt polarity, push-pull/open-drain configuration
#define FXOS8700_CTRL_REG4           0x2D // Interrupt enable register
#define FXOS8700_CTRL_REG5           0x2E // Interrupt pin (INT1/INT2) map
#define FXOS8700_OFF_X               0x2F // X-axis accelerometer offset adjust
#define FXOS8700_OFF_Y               0x30 // Y-axis accelerometer offset adjust
#define FXOS8700_OFF_Z               0x31 // Z-axis accelerometer offset adjust
#define FXOS8700_M_DR_STATUS         0x32 // Magnetic data ready
#define FXOS8700_M_OUT_X_MSB         0x33 // MSB of 16-bit magnetic data for X-axis
#define FXOS8700_M_OUT_X_LSB         0x34 // LSB of 16-bit magnetic data for X-axis
#define FXOS8700_M_OUT_Y_MSB         0x35 // MSB of 16-bit magnetic data for Y-axis
#define FXOS8700_M_OUT_Y_LSB         0x36 // LSB of 16-bit magnetic data for Y-axis
#define FXOS8700_M_OUT_Z_MSB         0x37 // MSB of 16-bit magnetic data for Z-axis
#define FXOS8700_M_OUT_Z_LSB         0x38 // LSB of 16-bit magnetic data for Z-axis
#define FXOS8700_CMP_X_MSB           0x39 // Bits [13:8] of integrated X-axis accerleration data
#define FXOS8700_CMP_X_LSB           0x3A // Bits [7:0] of integrated X-axis accerleration data
#define FXOS8700_CMP_Y_MSB           0x3B // Bits [13:8] of integrated Y-axis accerleration data
#define FXOS8700_CMP_Y_LSB           0x3C // Bits [7:0] of integrated Y-axis accerleration data
#define FXOS8700_CMP_Z_MSB           0x3D // Bits [13:8] of integrated Z-axis accerleration data
#define FXOS8700_CMP_Z_LSB           0x3E // Bits [7:0] of integrated Z-axis accerleration data
#define FXOS8700_M_OFF_X_MSB         0x3F // MSB of magnetometer of X-axis offset
#define FXOS8700_M_OFF_X_LSB         0x40 // LSB of magnetometer of X-axis offset
#define FXOS8700_M_OFF_Y_MSB         0x41 // MSB of magnetometer of Y-axis offset
#define FXOS8700_M_OFF_Y_LSB         0x42 // LSB of magnetometer of Y-axis offset
#define FXOS8700_M_OFF_Z_MSB         0x43 // MSB of magnetometer of Z-axis offset
#define FXOS8700_M_OFF_Z_LSB         0x44 // LSB of magnetometer of Z-axis offset
#define FXOS8700_MAX_X_MSB           0x45 // Magnetometer X-axis maximum value MSB
#define FXOS8700_MAX_X_LSB           0x46 // Magnetometer X-axis maximum value LSB
#define FXOS8700_MAX_Y_MSB           0x47 // Magnetometer Y-axis maximum value MSB
#define FXOS8700_MAX_Y_LSB           0x48 // Magnetometer Y-axis maximum value LSB
#define FXOS8700_MAX_Z_MSB           0x49 // Magnetometer Z-axis maximum value MSB
#define FXOS8700_MAX_Z_LSB           0x4A // Magnetometer Z-axis maximum value LSB
#define FXOS8700_MIN_X_MSB           0x4B // Magnetometer X-axis minimum value MSB
#define FXOS8700_MIN_X_LSB           0x4C // Magnetometer X-axis minimum value LSB
#define FXOS8700_MIN_Y_MSB           0x4D // Magnetometer Y-axis minimum value MSB
#define FXOS8700_MIN_Y_LSB           0x4E // Magnetometer Y-axis minimum value LSB
#define FXOS8700_MIN_Z_MSB           0x4F // Magnetometer Z-axis minimum value MSB
#define FXOS8700_MIN_Z_LSB           0x50 // Magnetometer Z-axis minimum value LSB
#define FXOS8700_TEMP                0x51 // Device temperature, valid range of -128 to 127 Â°C when M_CTRL1[m_hms] > 0b00
#define FXOS8700_M_THS_CFG           0x52 // Magnetic threshold detection function configuration
#define FXOS8700_M_THS_SRC           0x53 // Magnetic threshold event source register
#define FXOS8700_M_THS_X_MSB         0x54 // X-axis magnetic threshold MSB
#define FXOS8700_M_THS_X_LSB         0x55 // X-axis magnetic threshold LSB
#define FXOS8700_M_THS_Y_MSB         0x56 // Y-axis magnetic threshold MSB
#define FXOS8700_M_THS_Y_LSB         0x57 // Y-axis magnetic threshold LSB
#define FXOS8700_M_THS_Z_MSB         0x58 // Z-axis magnetic threshold MSB
#define FXOS8700_M_THS_Z_LSB         0x59 // Z-axis magnetic threshold LSB
#define FXOS8700_M_THS_COUNT         0x5A // Magnetic threshold debounce counter
#define FXOS8700_M_CTRL_REG1         0x5B // Control for magnetic sensor functions
#define FXOS8700_M_CTRL_REG2         0x5C // Control for magnetic sensor functions
#define FXOS8700_M_CTRL_REG3         0x5D // Control for magnetic sensor functions
#define FXOS8700_M_INT_SRC           0x5E // Magnetic interrupt source
#define FXOS8700_A_VECM_CFG          0x5F // Acceleration vector-magnitude configuration register
#define FXOS8700_A_VECM_THS_MSB      0x60 // Acceleration vector-magnitude threshold MSB
#define FXOS8700_A_VECM_THS_LSB      0x61 // Acceleration vector-magnitude threshold LSB
#define FXOS8700_A_VECM_CNT          0x62 // Acceleration vector-magnitude debounce count
#define FXOS8700_A_VECM_INITX_MSB    0x63 // Acceleration vector-magnitude X-axis reference value MSB
#define FXOS8700_A_VECM_INITX_LSB    0x64 // Acceleration vector-magnitude X-axis reference value LSB
#define FXOS8700_A_VECM_INITY_MSB    0x65 // Acceleration vector-magnitude Y-axis reference value MSB
#define FXOS8700_A_VECM_INITY_LSB    0x66 // Acceleration vector-magnitude Y-axis reference value LSB
#define FXOS8700_A_VECM_INITZ_MSB    0x67 // Acceleration vector-magnitude Z-axis reference value MSB
#define FXOS8700_A_VECM_INITZ_LSB    0x68 // Acceleration vector-magnitude Z-axis reference value LSB
#define FXOS8700_M_VECM_CFG          0x69 // Magnetic vector-magnitude configuration register
#define FXOS8700_M_VECM_THS_MSB      0x6A // Magnetic vector-magnitude threshold MSB
#define FXOS8700_M_VECM_THS_LSB      0x6B // Magnetic vector-magnitude threshold LSB
#define FXOS8700_M_VECM_CNT          0x6C // Magnetic vector-magnitude debounce count
#define FXOS8700_M_VECM_INITX_MSB    0x6D // Magnetic vector-magnitude reference value X-axis MSB
#define FXOS8700_M_VECM_INITX_LSB    0x6E // Magnetic vector-magnitude reference value X-axis LSB
#define FXOS8700_M_VECM_INITY_MSB    0x6F // Magnetic vector-magnitude reference value Y-axis MSB
#define FXOS8700_M_VECM_INITY_LSB    0x70 // Magnetic vector-magnitude reference value Y-axis LSB
#define FXOS8700_M_VECM_INITZ_MSB    0x71 // Magnetic vector-magnitude reference value Z-axis MSB
#define FXOS8700_M_VECM_INITZ_LSB    0x72 // Magnetic vector-magnitude reference value Z-axis LSB
#define FXOS8700_A_FFMT_THS_X_MSB    0x73 // X-axis FMT threshold MSB
#define FXOS8700_A_FFMT_THS_X_LSB    0x74 // X-axis FFMT threshold LSB
#define FXOS8700_A_FFMT_THS_Y_MSB    0x75 // Y-axis FFMT threshold MSB
#define FXOS8700_A_FFMT_THS_Y_LSB    0x76 // Y-axis FFMT threshold LSB
#define FXOS8700_A_FFMT_THS_Z_MSB    0x77 // Z-axis FFMT threshold MSB
#define FXOS8700_A_FFMT_THS_Z_LSB    0x78 // Z-axis FFMT threshold LSB

#define FXAS21002_I2C_ADDR0          0x20 // SA0 = Gnd
#define FXAS21002_I2C_ADDR1          0x21 // SA0 = Vcc
#define FXAS21002_STATUS             0x00 // Alias for DR_STATUS or F_STATUS
#define FXAS21002_OUT_X_MSB          0x01 // MSB of 16 bit X-axis data sample
#define FXAS21002_OUT_X_LSB          0x02 // LSB of 16 bit X-axis data sample
#define FXAS21002_OUT_Y _MSB         0x03 // MSB of 16 bit Y-axis data sample
#define FXAS21002_OUT_Y_LSB          0x04 // LSB of 16 bit Y-axis data sample
#define FXAS21002_OUT_Z_MSB          0x05 // MSB of 16 bit Z-axis data sample
#define FXAS21002_OUT_Z_LSB          0x06 // LSB of 16 bit Z-axis data sample
#define FXAS21002_DR_STATUS          0x07 // Data-ready status information
#define FXAS21002_F_STATUS           0x08 // FIFO Status
#define FXAS21002_F_SETUP            0x09 // FIFO setup
#define FXAS21002_F_EVENT            0x0A // FIFO event
#define FXAS21002_INT_SRC_FLAG       0x0B // Interrupt event source status flags
#define FXAS21002_WHO_AM_I           0x0C // Device ID
#define FXAS21002_CTRL_REG0          0x0D // Full-scale range selection, high-pass filter setting, SPI mode selection
#define FXAS21002_RT_CFG             0x0E // Rate threshold function configuration
#define FXAS21002_RT_SRC             0x0F // Rate threshold event flags status register
#define FXAS21002_RT_THS             0x10 // Rate threshold function threshold register
#define FXAS21002_RT_COUNT           0x11 // Rate threshold function debounce counter
#define FXAS21002_TEMP               0x12 // Device temperature in °C
#define FXAS21002_CTRL_REG1          0x13 // Operating mode, ODR, self-test and soft reset
#define FXAS21002_CTRL_REG2          0x14 // Interrupt configuration
#define FXAS21002_CTRL_REG3          0x15 // Auto-increment address configuration, external power control, FSR expansion

#define MPL3115_I2C_ADDR             0x60 // fixed I2C address
#define MPL3115_STATUS               0x00 // Sensor Status Register
#define MPL3115_OUT P_MSB            0x01 // Pressure Data Out MSB
#define MPL3115_OUT_P_CSB            0x02 // Pressure Data Out CSB
#define MPL3115_OUT_P_LSB            0x03 // Pressure Data Out LSB
#define MPL3115_OUT_T_MSB            0x04 // Temperature Data Out MSB
#define MPL3115_OUT_T_LSB            0x05 // Temperature Data Out LSB
#define MPL3115_DR_STATUS            0x06 // Sensor Status Register
#define MPL3115_OUT_P_DELTA MSB      0x07 // Pressure Data Out Delta MSB
#define MPL3115_OUT_P_DELTA_CSB      0x08 // Pressure Data Out Delta CSB
#define MPL3115_OUT_P_DELTA_LSB      0x09 // Pressure Data Out Delta LSB
#define MPL3115_OUT_T_DELTA_MSB      0x0A // Temperature Data Out Delta MSB
#define MPL3115_OUT_T_DELTA_LSB      0x0B // Temperature Data Out Delta LSB
#define MPL3115_WHO_AM_I             0x0C // Device Identification Register
#define MPL3115_F_STATUS             0x0D // FIFO Status Register
#define MPL3115_F_DATA               0X0E // FIFO 8-bit Data Access
#define MPL3115_F_SETUP              0x0F // FIFO Setup Register
#define MPL3115_TIME_DLY             0x10 // Time Delay
#define MPL3115_SYSMOD               0x11 // System Mode Register
#define MPL3115_INT_SOURCE           0x12 // Interrupt Source Register
#define MPL3115_PT_DATA_CFG          0x13 // PT Data Configuration Register
#define MPL3115_BAR_IN_MSB           0x14 // BAR Input in MSB
#define MPL3115_BAR_IN_LSB           0x15 // BAR Input in LSB
#define MPL3115_P_TGT_MSB            0x16 // Pressure Target MSB
#define MPL3115_P_TGT_LSB            0x17 // Pressure Target LSB
#define MPL3115_T_TGT                0x18 // Temperature Target
#define MPL3115_P_WND_MSB            0x19 // Pressure/Altitude Window MSB
#define MPL3115_P_WND_LSB            0x1A // Pressure/Altitude Window LSB
#define MPL3115_T_WND                0X1B // Temperature Window
#define MPL3115_P_MIN_MSB            0x1C // Minimum Pressure Data Out MSB
#define MPL3115_P_MIN_CSB            0x1D // Minimum Pressure Data Out CSB
#define MPL3115_P_MIN_LSB            0x1E // Minimum Pressure Data Out LSB
#define MPL3115_T_MIN_MSB            0x1F // Minimum Temperature Data Out MSB
#define MPL3115_T_MIN_LSB            0x20 // Minimum Temperature Data Out LSB
#define MPL3115_P_MAX_MSB            0x21 // Maximum Pressure Data Out MSB
#define MPL3115_P_MAX_CSB            0x22 // Maximum Pressure Data Out CSB
#define MPL3115_P_MAX_LSB            0x23 // Maximum Pressure Data Out LSB
#define MPL3115_T_MAX_MSB            0X24 // Maximum Temperature Data Out MSB
#define MPL3115_T_MAX_LSB            0x25 // Maximum Temperature Data Out LSB
#define MPL3115_CTRL_REG1            0x26 // Control Register 1
#define MPL3115_CTRL_REG2            0x27 // Control Register 2
#define MPL3115_CTRL_REG3            0x28 // Control Register 3
#define MPL3115_CTRL_REG4            0x29 // Control Register 4
#define MPL3115_CTRL_REG5            0x2A // Control Register 5
#define MPL3115_OFF_P                0x2B // Pressure Data User Offset
#define MPL3115_OFF_T                0x2C // Temperature Data User Offset
#define MPL3115_OFF_H                0x2D // Altitude Data User Offset Register

