const double SPLIT = 0.01;
const int COUNT = 200;
const double PI = 3.14159265;
const int INTERVAL = 300;
const int PERIOD = 60 * 1000; //60,000 ms
const double FACTOR = 2 * PI / PERIOD;
const double GAP_LINEAR = 100;
const double GAP = (double)PERIOD / COUNT;
typedef double Func(double);


static double Ratio = 0.7;

#define GetCPUTickCount() _rdtsc()
