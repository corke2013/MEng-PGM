#ifndef DIS_MODEL_HPP
#define DIS_MODEL_HPP

// standard headers
#include <map>
#include <vector>
#include <variant>

// emdw headers
#include <factor.hpp>
#include <discretetable.hpp>

// patrec headers
#include <prlite_vector.hpp>

// custom headers
#include <utils.hpp>

class DisModel
{
public:
	DisModel(std::vector<rcptr<Factor>>* factors, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>, double>> modelParameters);
	void buildDisModel(unsigned int timeStep, std::map<std::size_t, std::tuple<unsigned int, unsigned int, prlite::ColVector<double>, prlite::RowMatrix<double>>> timeStepData, std::vector<std::tuple<std::size_t, unsigned int, unsigned int>>& rvIds);
private:
	std::vector<rcptr<Factor>>* factors;
	std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>, double>> modelParameters;
};
#endif // !DIS_MODEL_HPP
