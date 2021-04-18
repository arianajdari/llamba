#ifndef __SETTINGS_HPP__
#define __SETTINGS_HPP__

#include <fstream>

#include <llamba/settings/affinity.hpp>
#include <llamba/settings/time_format.hpp>
#include <llamba/settings/scheduling_strategy.hpp>
#include <llamba/settings/parallelization_strategy.hpp>

#include <llamba/overloading/overloading.hpp>

namespace llamba
{

struct settings
{
    public:
        
        settings() 
        {
            this->THREAD_NUMBER            =     2_THREADS;
            this->SCHED_STRATEGY           =     SCHEDULING_STRATEGY::SCHEDULING_TS;
            this->PRIORITY                 =     19_PRIORITY;
            this->AFFINITY                 =     AFFINITY::FALSE;
            this->PARALLELIZATION_STRATEGY =     PARALLELIZATION_STRATEGY::PTHREADS;
        }

        settings(int parallelization_strategy_) :
            THREAD_NUMBER(0),
            SCHED_STRATEGY(SCHEDULING_STRATEGY::SCHEDULING_TS),
            PRIORITY(19_PRIORITY),
            AFFINITY(AFFINITY::FALSE),
            PARALLELIZATION_STRATEGY(parallelization_strategy_)
        {

        }

        settings(int thread_number_, int parallelization_strategy_) :
            THREAD_NUMBER(thread_number_),
            SCHED_STRATEGY(SCHEDULING_STRATEGY::SCHEDULING_TS),
            PRIORITY(19_PRIORITY),
            AFFINITY(AFFINITY::FALSE),
            PARALLELIZATION_STRATEGY(parallelization_strategy_)
        {

        }

        settings(int thread_number_, int sched_strategy_, int priority_, bool affinity_, std::vector<int> logical_cpus_, int parallelization_strategy_) :
            THREAD_NUMBER(thread_number_),
            SCHED_STRATEGY(sched_strategy_),
            PRIORITY(priority_),
            AFFINITY(affinity_),
            LOGICAL_CPUS(logical_cpus_),
            PARALLELIZATION_STRATEGY(parallelization_strategy_)
        {

        }

        settings(int thread_number_, int sched_strategy_, int priority_, bool affinity_, int parallelization_strategy_) :
            THREAD_NUMBER(thread_number_),
            SCHED_STRATEGY(sched_strategy_),
            PRIORITY(priority_),
            AFFINITY(affinity_),
            PARALLELIZATION_STRATEGY(parallelization_strategy_)
        {

        }

        static settings make_settings()
        {
            settings settings_ =
                settings(4_THREADS, PARALLELIZATION_STRATEGY::PTHREADS);

            return settings_;
        }

        static settings make_settings(int thread_number_, int sched_strategy_, int priority_, bool affinity_, int parallelization_strategy_)
        {
            settings settings_ =
                settings(thread_number_, sched_strategy_, priority_, affinity_, parallelization_strategy_);
            
            return settings_;
        }

        int  THREAD_NUMBER;
        int  SCHED_STRATEGY;
        int  PRIORITY;
        bool AFFINITY;
        std::vector<int> LOGICAL_CPUS;
        int  PARALLELIZATION_STRATEGY;

    private:

        const char * get_scheduling_strategy(int scheduling_strategy) const
        {
            if(scheduling_strategy == SCHEDULING_STRATEGY::SCHEDULING_RR)
                return "\"RR\"";
            else if(scheduling_strategy == SCHEDULING_STRATEGY::SCHEDULING_FIFO)
                return "\"FIFO\"";
            
            return "\"TS\"";
        }

        const char * get_affinity(bool affinity_) const
        {
            if(affinity_)
                return "\"true\"";
            
            return "\"false\"";
        }

        const char * get_parallelization_strategy(int parallelization_strategy) const
        {
            if(parallelization_strategy == PARALLELIZATION_STRATEGY::OPENMP)
                return "\"OPENMP\"";
            else if(parallelization_strategy == PARALLELIZATION_STRATEGY::SERIAL)
                return "\"SERIAL\"";

            return "\"PTHREADS\"";
        }

        friend std::ostream& operator<<(std::ostream& os, const settings& settings_);
};

std::ostream& operator<<(std::ostream& os, const settings& settings_)
{
    os << "[" << settings_.THREAD_NUMBER                                                    << ", "
              << settings_.get_scheduling_strategy(settings_.SCHED_STRATEGY)                << ", "
              << settings_.PRIORITY                                                         << ", "
              << settings_.get_affinity(settings_.AFFINITY)                                 << ", " 
              << settings_.get_parallelization_strategy(settings_.PARALLELIZATION_STRATEGY) << "]";

    return os;
}

}

#endif