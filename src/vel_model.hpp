#ifndef VEL_MODEL_HPP
#define VEL_MODEL_HPP

// standard headers
#include <map>
#include <vector>
#include <variant>

// emdw headers
#include <factor.hpp>
#include <discretetable.hpp>

// custom headers
#include <utils.hpp>

class VelModel
{
public:
	VelModel(std::vector<rcptr<Factor>>* factors, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>, double>> modelParameters);
	void buildVelModel(unsigned int timeStep, std::map<unsigned int, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>> timeStepDataa);
private:
	std::vector<rcptr<Factor>>* factors;
	std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>, double>> modelParameters;
};
#endif // !VEL_MODEL_HPP
