#ifndef TRIFIT_HPP_
#define TRIFIT_HPP_

struct TriFit {
public:
	double saturation;
	double brightness;
	double error;
	TriFit(double saturation, double brightness, double error) : saturation(saturation), brightness(brightness), error(error) { };
};

#endif
