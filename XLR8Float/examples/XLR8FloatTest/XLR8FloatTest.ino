#include <XLR8Float.h>

/* XLR8FloatTest
 Copyright (c) 2016 Alorim Technology.  All right reserved.
 Written by Matt Weber (linkedin.com/in/mattweberdesign) of
 Alorium Technology (info@aloriumtech.com)
 Demonstrates usage of XLR8Float library and
  hardware accelerators
 Loops through random numbers and updates statistics
  two different ways
  1 - Using software floating point math
  2 - Using XLR8 hardware accelerated floating point math
 The time to update the statistics is measured
  and statistics on that are calculated and periodically
  reported.
 Statistics kept are count, min, max, mean, 
  and variance. Variance is calculated using the algorithm
  from Knuth/Welford. Count, min and max are kept as ints. Mean
  and M2 (sum of squares of differences from
  the running mean, used in variance calculation) are
  floats. 
*/

int randNumber;

// Hold a set of statistics
struct statistics {
  float mean = 0.0;
  float M2 = 0.0;
  // float variance; // calculate as M2/(count-1) only when needed
  uint32_t count = 0;
  int   maximum = -32768;
  int   minimum = 32767;
};

struct statistics stats_float;
struct statistics stats_xlr8;
struct statistics runstats_float;
struct statistics runstats_xlr8;


void setup() {
  Serial.begin(115200);
  //Serial.begin(9600);

  // Different random seeds
  randomSeed(1);
  
  // Shut off interrupts (timer used for micros() and millis())
  //  so they don't affect the runtimes being measured
  cli();
  
  // Measure number of cycles needed to do calculations by using 
  //   timer/counter 1
  TCCR1A = 0;
  TCCR1B = (1 << CS10) ; // no prescaler
  TCNT1 = 0;
  
}

void loop() {
  // Generate 16 bit signed integer random number centered on zero
  randNumber = (int)random(-32767,32767);
  //randNumber = (int)random(-32,32); // use smaller range to drive the mean closer to zero
  // Incorporate this number into the statistics
  //  using both XLR8 and standard routines. Both
  //  should give the same result
  int runtimeFloat = update_stats_float(randNumber,&stats_float);
  int runtimeXLR8  = update_stats_xlr8(randNumber,&stats_xlr8);
  // Compare to verify XLR8 is giving same results as software floating point
  //   (typically don't use == on floating point compare but in this case
  //    we really want to know that every bit is identical)
  bool printMismatch = false;
  if (stats_float.count != stats_xlr8.count) {printMismatch = true;}
  if (stats_float.minimum != stats_xlr8.minimum) {printMismatch = true;}
  if (stats_float.maximum != stats_xlr8.maximum) {printMismatch = true;}
  if (stats_float.mean != stats_xlr8.mean) {printMismatch = true;}
  if (stats_float.M2 != stats_xlr8.M2) {printMismatch = true;}
  if (printMismatch) {
    GPIOR1 = 0x80; // flag to finish with failure 
    Serial.println("MISMATCH    xlr8    expected");
    Serial.print("  count  ");Serial.print(stats_xlr8.count);Serial.print("  ");Serial.println(stats_float.count);
    Serial.print("  min    ");Serial.print(stats_xlr8.minimum);Serial.print("  ");Serial.println(stats_float.minimum);
    Serial.print("  max    ");Serial.print(stats_xlr8.maximum);Serial.print("  ");Serial.println(stats_float.maximum);
    Serial.print("  mean   ");Serial.print(stats_xlr8.mean);Serial.print("  ");Serial.println(stats_float.mean);
    Serial.print("  M2     ");Serial.print(stats_xlr8.M2);Serial.print("  ");Serial.println(stats_float.M2);
  }
  // Update statistics on the runtime of the routines
  update_stats_xlr8(runtimeFloat,&runstats_float);
  update_stats_xlr8(runtimeXLR8,&runstats_xlr8);
  // Periodically print runtime statistics to serial
  if (!(runstats_float.count & 0xFFF)) {
    GPIOR1 = (GPIOR1 == 0) ? 0xC0 : 0x80; // finish flag with either pass or fail status
    Serial.println("RUNTIME (clocks)    xlr8     software");
    Serial.print("     count          ");Serial.print(runstats_xlr8.count);Serial.print("    ");Serial.println(runstats_float.count);
    Serial.print("     min              ");Serial.print(runstats_xlr8.minimum);Serial.print("     ");Serial.println(runstats_float.minimum);
    Serial.print("     max              ");Serial.print(runstats_xlr8.maximum);Serial.print("     ");Serial.println(runstats_float.maximum);
    Serial.print("     mean             ");Serial.print(runstats_xlr8.mean);Serial.print("  ");Serial.println(runstats_float.mean);
    //Serial.print("     M2             ");Serial.print(runstats_xlr8.M2);Serial.print("  ");Serial.println(runstats_float.M2);
    float xlr8stddev = sqrt(runstats_xlr8.M2/((float)runstats_xlr8.count - 1));
    float floatstddev = sqrt(runstats_float.M2/((float)runstats_float.count - 1));
    Serial.print("     stddev             ");Serial.print(xlr8stddev);Serial.print("    ");Serial.println(floatstddev);    
  }
  if (runstats_xlr8.count >= 65536) {
    Serial.println("Random Number Statistics");
    Serial.print("  count          ");Serial.println(stats_xlr8.count);//Serial.print("  ");Serial.println(stats_float.count);
    Serial.print("  min           ");Serial.println(stats_xlr8.minimum);//Serial.print("  ");Serial.println(stats_float.minimum);
    Serial.print("  max            ");Serial.println(stats_xlr8.maximum);//Serial.print("  ");Serial.println(stats_float.maximum);
    Serial.print("  mean           ");Serial.println(stats_xlr8.mean);//Serial.print("  ");Serial.println(stats_float.mean);
    //Serial.print("  M2           ");Serial.println(stats_xlr8.M2);//Serial.print("  ");Serial.println(stats_float.M2);
    float xlr8stddevx = sqrt(stats_xlr8.M2/((float)stats_xlr8.count - 1));
    float floatstddevx = sqrt(stats_float.M2/((float)stats_float.count - 1));
    Serial.print("  stddev         ");Serial.println(xlr8stddevx);//Serial.print("    ");Serial.println(floatstddevx);
    // Expected stddev for a uniform distribution is sqrt( ((range+1)^2 - 1)/12 ) , with a large range this is roughly range/sqrt(12)
    Serial.println();
    Serial.print("XLR8 Floating Point measured ");
    float speedup = runstats_float.mean/runstats_xlr8.mean;
    Serial.print(speedup);
    Serial.println(" times faster than ordinary Arduino Floating Point");
    Serial.flush();
    while (1); // stop here
  }
}

// input new value to incorporate into the state at index
// return the time needed to do the calculations
// default operators will use software floating point
int update_stats_float (int newval, struct statistics* stats) {
  float x, delta1,temp,delta2;
  TCNT1 = 0;
  if (newval > stats->maximum) {stats->maximum = newval;}
  if (newval < stats->minimum) {stats->minimum = newval;}
  stats->count++; 
  TCNT1 = 0; // reset timer if want to measure just the floating point time
  x = (float)newval;
  delta1 = x - stats->mean; 
  temp   = delta1/(float)stats->count;
  stats->mean   = temp + stats->mean;
  delta2 = x - stats->mean; 
  temp   = delta2 * delta1; 
  stats->M2     = temp + stats->M2;
  //variance = stats->M2 / (stats->count - 1);
  return TCNT1;
}

int update_stats_xlr8 (int newval, struct statistics* stats) {
  float x, delta1,temp,delta2;
  TCNT1 = 0;
  if (newval > stats->maximum) {stats->maximum = newval;}
  if (newval < stats->minimum) {stats->minimum = newval;}
  stats->count++;
  TCNT1 = 0; // reset timer if want to measure just the floating point time
  x = xlr8FloatFromInt(newval);
  delta1   = xlr8FloatSub(x,stats->mean);       // d1=x-mean
  temp     = xlr8FloatDiv(delta1,(float)stats->count); // t = d1/n 
  stats->mean     = xlr8FloatAdd(temp,stats->mean);    // mean=t+mean
  delta2   = xlr8FloatSub(x,stats->mean);       // d2=x-mean
  temp     = xlr8FloatMult(delta2,delta1); // t=d2*d1
  stats->M2       = xlr8FloatAdd(temp,stats->M2);      // m2=m2+t
  //variance = xlr8FloatDiv(M2,prevcount); // v=m2/t
  return TCNT1;
}

