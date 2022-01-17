#ifndef FABRYKA_IMPLEMENTATION_REPORTS_HPP
#define FABRYKA_IMPLEMENTATION_REPORTS_HPP

#include "factory.hpp"


class IntervalReportNotifier{
public:

    IntervalReportNotifier(TimeOffset to) : TimeDivider_(to) {};

    bool should_generate_report(Time t) { return (t-1) % TimeDivider_ == 0; };

private:

    Time TimeDivider_;

};



class SpecificTurnsReportNotifier{
public:

    SpecificTurnsReportNotifier(std::set<Time> turns) : TimeStamps_(turns) {};

    bool should_generate_report(Time t) { return (TimeStamps_.count(t) > 0); };

private:

    std::set<Time> TimeStamps_;

};


void generate_structure_report(const Factory& f, std::ostream& os);


void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t);


#endif
