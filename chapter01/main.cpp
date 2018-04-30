
#include "header.h"
#include "const.h"
using namespace std;


inline DWORD get_time() { return GetTickCount(); }

double calc_sin(double x) {  return (1 + sin(FACTOR * x)) / 2;}

void set_ratio()
{
  double ret = 0.0;
  printf("Ratio:([0,1]) ");
  scanf("%lf", &ret);
  if (ret < 0.0 || ret > 1.0) ret = 0.5;
  Ratio = ret;
}

void solve_nonperiod(Func *calc)
{
  double tb = 0;
  while(1) {
    unsigned ta = get_time();
    double r = calc(tb);
    if (r < 0 || r > 1) r = 1;
    DWORD busy = r * GAP;
    while(get_time() - ta < busy) {}
    Sleep(GAP - busy);
    //tb += GAP;
    tb += get_time() - ta;
  }
}

void solve_period(Func *calc)
{
  double x = 0.0;
  double cache[COUNT];
  for (int i = 0; i < COUNT; ++i, x += GAP)
        {
            cache[i] = calc(x); //cout<<cache[i]<<endl>>;
        }
  int count = 0;
  while(1) {
    unsigned ta = get_time();
    if (count >= COUNT) count = 0;
    double r = cache[count++];
    DWORD busy = r * GAP;
    while(get_time() - ta < busy) {}
    Sleep(GAP - busy);
  }
}

void solve_linear(Func*)
{
  const unsigned BUSY =  Ratio * GAP_LINEAR;
  const unsigned IDLE = (1 - Ratio) * GAP_LINEAR;
  while(1) {
    unsigned ta = get_time();
    while(get_time() - ta < BUSY) {}
    Sleep(IDLE);
  }
}

void run(unsigned index = 0, double ratio = -1.0)
{
  typedef void Solve(Func *calc);
  Func *func[] = { calc_sin};
  Func *calc = func[0];
  Solve *solve_func[] = { solve_linear, solve_period, solve_nonperiod };
  if (index >= sizeof(solve_func) / sizeof(solve_func[0])) index = 0;
  Solve *solve = solve_func[index];
  if (solve == solve_linear) {
    if (ratio >= 0 && ratio <= 1) Ratio = ratio;
    else set_ratio();
  }
  const int MAX_CPUS = 32;
  HANDLE handle[MAX_CPUS];
  DWORD thread_id[MAX_CPUS];
  SYSTEM_INFO info;
  GetSystemInfo(&info);
  const int num = info.dwNumberOfProcessors;
  for (int i = 0; i < num; ++i) {
    if ((handle[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)solve,
                                    (VOID*)calc, 0, &thread_id[i])) != NULL)
      SetThreadAffinityMask(handle[i], i);
  }
  WaitForSingleObject(handle[0],INFINITE);
}
void EasycontrlCpu()
{
    for(;;)
    {
        for(int i = 0; i < 96000000; i++);
        Sleep(100);
    }
}

void TickControlCpu()
{
    //EasycontrlCpu();
    int busyTime = 10;
    int idleTime = busyTime;

    int64_t startTime = 0;
    while(true)
    {
        startTime = GetTickCount();
        //busy loop
        while((GetTickCount() - startTime) <= busyTime);

        //idle loop
        Sleep(idleTime);
    }
    cout<<GetTickCount()<<endl;
}

void  drawsin()
{
    DWORD busytime[COUNT];
    DWORD idletime[COUNT];

    int half = INTERVAL / 2;
    double radian = 0.0;
    for(int i = 0; i < COUNT; i++)
    {
        busytime[i] = (DWORD)(half + (sin(PI * radian) * half));
        idletime[i] = INTERVAL - busytime[i];
        radian += SPLIT;
    }

    DWORD starttime = 0;
    int j = 0;
    while(true)
    {
        j = j % COUNT;
        starttime = GetTickCount();
        while((GetTickCount() - starttime) <= busytime[j]);
        Sleep(idletime[j]);
        j++;
    }
}


int main()
{
    //int temp;
    //SetThreadAffinityMask(GetCurrentProcess(), 0x00000000);
    //EasycontrlCpu();
    //SetThreadAffinityMask(GetCurrentProcess(), 0x00000001);
    //TickControlCpu();
    //drawsin();
    run(1,0.5);
    //cin >> temp;
    return 0;
}





