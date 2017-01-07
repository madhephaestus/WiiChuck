#ifndef _NXP_Motion_Sensors_
#define _NXP_Motion_Sensors_

#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>

// TODO: move these inside class namespace
#define G_PER_COUNT            0.0001220703125f  // = 1/8192
#define DEG_PER_SEC_PER_COUNT  0.0625f  // = 1/16
#define UT_PER_COUNT           0.1f

class NXPMotionSense {
public:
	bool begin();
	bool available() {
		update();
		if (newdata) return true;
		return false;
	}
	void readMotionSensor(int& ax, int& ay, int& az, int& gx, int& gy, int& gz) {
		if (!newdata) update();
		newdata = 0;
		ax = accel_mag_raw[0];
		ay = accel_mag_raw[1];
		az = accel_mag_raw[2];
		gx = gyro_raw[0];
		gy = gyro_raw[1];
		gz = gyro_raw[2];
	}
	void readMotionSensor(int& ax, int& ay, int& az, int& gx, int& gy, int& gz, int& mx, int& my, int& mz) {
		if (!newdata) update();
		newdata = 0;
		ax = accel_mag_raw[0];
		ay = accel_mag_raw[1];
		az = accel_mag_raw[2];
		mx = accel_mag_raw[3];
		my = accel_mag_raw[4];
		mz = accel_mag_raw[5];
		gx = gyro_raw[0];
		gy = gyro_raw[1];
		gz = gyro_raw[2];
	}
	void readMotionSensor(float& ax, float& ay, float& az, float& gx, float& gy, float& gz) {
		if (!newdata) update();
		newdata = 0;
		ax = (float)accel_mag_raw[0] * G_PER_COUNT;
		ay = (float)accel_mag_raw[1] * G_PER_COUNT;
		az = (float)accel_mag_raw[2] * G_PER_COUNT;
		gx = (float)gyro_raw[0] * DEG_PER_SEC_PER_COUNT;
		gy = (float)gyro_raw[1] * DEG_PER_SEC_PER_COUNT;
		gz = (float)gyro_raw[2] * DEG_PER_SEC_PER_COUNT;
	}
	void readMotionSensor(float& ax, float& ay, float& az, float& gx, float& gy, float& gz, float& mx, float& my, float& mz) {
		if (!newdata) update();
		newdata = 0;
		ax = (float)accel_mag_raw[0] * G_PER_COUNT - cal[0];
		ay = (float)accel_mag_raw[1] * G_PER_COUNT - cal[1];
		az = (float)accel_mag_raw[2] * G_PER_COUNT - cal[2];
		gx = (float)gyro_raw[0] * DEG_PER_SEC_PER_COUNT - cal[3];
		gy = (float)gyro_raw[1] * DEG_PER_SEC_PER_COUNT - cal[4];
		gz = (float)gyro_raw[2] * DEG_PER_SEC_PER_COUNT - cal[5];
		float x = (float)accel_mag_raw[3] * UT_PER_COUNT - cal[6];
		float y = (float)accel_mag_raw[4] * UT_PER_COUNT - cal[7];
		float z = (float)accel_mag_raw[5] * UT_PER_COUNT - cal[8];
		mx = x * cal[10] + y * cal[13] + z * cal[14];
		my = x * cal[13] + y * cal[11] + z * cal[15];
		mz = x * cal[14] + y * cal[15] + z * cal[12];
	}

	bool writeCalibration(const void *data);
	void getCalibration(float *offsets, float *softiron=NULL, float *fieldstrength=NULL) {
		if (offsets != NULL) {
			memcpy(offsets, cal, 36);
		}
		if (softiron != NULL) {
			*softiron++ = cal[10];
			*softiron++ = cal[13];
			*softiron++ = cal[14];
			*softiron++ = cal[13];
			*softiron++ = cal[11];
			*softiron++ = cal[15];
			*softiron++ = cal[14];
			*softiron++ = cal[15];
			*softiron++ = cal[12];
		}
		if (fieldstrength != NULL) *fieldstrength = cal[9];
	}
private:
	void update();
	bool FXOS8700_begin();
	bool FXAS21002_begin();
	bool MPL3115_begin();
	bool FXOS8700_read(int16_t *data);
	bool FXAS21002_read(int16_t *data);
	bool MPL3115_read(int32_t *altitude, int16_t *temperature);
	float cal[16]; // 0-8=offsets, 9=field strength, 10-15=soft iron map
	int16_t accel_mag_raw[6];
	int16_t gyro_raw[3];
	int16_t temperature_raw;
	uint8_t newdata;
};


class NXPSensorFusion {
public:
	void begin(float sampleRate=100.0f);
	void update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
	// TODO: these aren't the same ranges as Madgwick & Mahony... or are they??
	float getRoll() { return PhiPl; }
	float getPitch() { return ThePl; }
	float getYaw() { return PsiPl; }
	typedef struct {
		float q0; // w
		float q1; // x
		float q2; // y
		float q3; // z
	} Quaternion_t;
	// These are Madgwick & Mahony - extrinsic rotation reference (wrong!)
	//float getPitch() {return atan2f(2.0f * qPl.q2 * qPl.q3 - 2.0f * qPl.q0 * qPl.q1, 2.0f * qPl.q0 * qPl.q0 + 2.0f * qPl.q3 * qPl.q3 - 1.0f);};
	//float getRoll() {return -1.0f * asinf(2.0f * qPl.q1 * qPl.q3 + 2.0f * qPl.q0 * qPl.q2);};
	//float getYaw() {return atan2f(2.0f * qPl.q1 * qPl.q2 - 2.0f * qPl.q0 * qPl.q3, 2.0f * qPl.q0 * qPl.q0 + 2.0f * qPl.q1 * qPl.q1 - 1.0f);};

private:
        float PhiPl;                    // roll (deg)
        float ThePl;                    // pitch (deg)
        float PsiPl;                    // yaw (deg)
        float RhoPl;                    // compass (deg)
        float ChiPl;                    // tilt from vertical (deg)
        // orientation matrix, quaternion and rotation vector
        float RPl[3][3];                // a posteriori orientation matrix
        Quaternion_t qPl;               // a posteriori orientation quaternion
        float RVecPl[3];                // rotation vector
        // angular velocity
        float Omega[3];                 // angular velocity (deg/s)
        // systick timer for benchmarking
        int32_t systick;                // systick timer;
        // end: elements common to all motion state vectors

        // elements transmitted over bluetooth in kalman packet
        float bPl[3];                   // gyro offset (deg/s)
        float ThErrPl[3];               // orientation error (deg)
        float bErrPl[3];                // gyro offset error (deg/s)
        // end elements transmitted in kalman packet

        float dErrGlPl[3];              // magnetic disturbance error (uT, global frame)
        float dErrSePl[3];              // magnetic disturbance error (uT, sensor frame)
        float aErrSePl[3];              // linear acceleration error (g, sensor frame)
        float aSeMi[3];                 // linear acceleration (g, sensor frame)
        float DeltaPl;                  // inclination angle (deg)
        float aSePl[3];                 // linear acceleration (g, sensor frame)
        float aGlPl[3];                 // linear acceleration (g, global frame)
        float gErrSeMi[3];              // difference (g, sensor frame) of gravity vector (accel) and gravity vector (gyro)
        float mErrSeMi[3];              // difference (uT, sensor frame) of geomagnetic vector (magnetometer) and geomagnetic vector (gyro)
        float gSeGyMi[3];               // gravity vector (g, sensor frame) measurement from gyro
        float mSeGyMi[3];               // geomagnetic vector (uT, sensor frame) measurement from gyro
        float mGl[3];                   // geomagnetic vector (uT, global frame)
        float QvAA;                     // accelerometer terms of Qv
        float QvMM;                     // magnetometer terms of Qv
        float PPlus12x12[12][12];       // covariance matrix P+
        float K12x6[12][6];             // kalman filter gain matrix K
        float Qw12x12[12][12];          // covariance matrix Qw
        float C6x12[6][12];             // measurement matrix C
        float RMi[3][3];                // a priori orientation matrix
        Quaternion_t Deltaq;            // delta quaternion
        Quaternion_t qMi;               // a priori orientation quaternion
        float casq;                     // FCA * FCA;
        float cdsq;                     // FCD * FCD;
        float Fastdeltat;               // sensor sampling interval (s) = 1 / SENSORFS
        float deltat;                   // kalman filter sampling interval (s) = OVERSAMPLE_RATIO / SENSORFS
        float deltatsq;                 // fdeltat * fdeltat
        float QwbplusQvG;               // FQWB + FQVG
        int8_t FirstOrientationLock;    // denotes that 9DOF orientation has locked to 6DOF
        int8_t resetflag;               // flag to request re-initialization on next pass
};


#undef G_PER_COUNT
#undef DEG_PER_SEC_PER_COUNT
#undef UT_PER_COUNT

#endif
