#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>    

using namespace std;
#define BOOST_DATE_TIME_SOURCE    


void time_elapsed()
{
    int sum = 0;

    boost::posix_time::ptime start_cpu_;
    boost::posix_time::ptime stop_cpu_;

    // 获取当前时间
    start_cpu_ = boost::posix_time::microsec_clock::local_time();

    for (int i = 0; i < 1e10; i++)
        sum += i;

    stop_cpu_ = boost::posix_time::microsec_clock::local_time();

    // 返回毫秒数
    float elapsed_milliseconds_ = (stop_cpu_ - start_cpu_).total_milliseconds();

    // 返回微秒数
    float elapsed_microseconds_ = (stop_cpu_ - start_cpu_).total_microseconds();

    cout << "takes " << elapsed_milliseconds_ << "ms"<< endl;
    cout << "takes " << elapsed_microseconds_ << "μs"<< endl;

}

void get_date_time()
{
    std::string str_time = boost::gregorian::to_iso_string(
                                    boost::gregorian::day_clock::local_day());    
        
    std::cout << str_time.c_str() << std::endl;  

            
    str_time = boost::posix_time::to_iso_string(    
                                boost::posix_time::second_clock::local_time());    
        
    // str_time 里存放时间的格式是YYYYMMDDTHHMMSS，日期和时间用大写字母T隔开了    
    std::cout << str_time.c_str() << std::endl;   
        
    int pos = str_time.find('T');    
    str_time.replace(pos, 1, std::string("-"));    
    str_time.replace(pos + 3, 0, std::string(":"));    
    str_time.replace(pos + 6, 0, std::string(":"));    
        
    std::cout << str_time.c_str() << std::endl;   
}

int main(int argc, char const *argv[])
{
    time_elapsed();
    
    get_date_time();
    
    return 0;
}