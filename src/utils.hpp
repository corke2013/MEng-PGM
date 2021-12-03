#ifndef UTILS_HPP
#define UTILS_HPP

//standard headers
#include<map>
#include <variant>

// prlite headers
#include <prlite_vector.hpp>
#include <prlite_matrix.hpp>

// custom headers
#include<json.hpp>

namespace Utils {
	std::size_t hashFromInts(unsigned int a, unsigned int b);
	std::size_t hashFromInts(unsigned int a, unsigned int b, unsigned int c);
	std::size_t hashFromInts(unsigned int a, unsigned int b, unsigned int c, unsigned int d);
	std::map<unsigned int, std::map<unsigned int, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>>> jsonDataToMapData(nlohmann::json jsonData);
	gLinear::gRowMatrix<double> vector2DToRowMatrix(std::vector<std::vector<double>> vector2D);
	std::vector<std::vector<double>> rowMatrixTo2DVector(gLinear::gMatrix<double, gLinear::ROW_DENSE> rowMatrix);
	double getDistance(double a, double b, double c, double d, double e, double f);
	double getLikelyhood(gLinear::gColVector<double> measMean, gLinear::gRowMatrix<double> measCov, gLinear::gColVector<double> modelMean, gLinear::gRowMatrix<double> modelCov);
}
#endif // !UTILS_HPP