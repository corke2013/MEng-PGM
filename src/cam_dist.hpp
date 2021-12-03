// cam_dist.hpp
#ifndef CAM_DIST_HPP
#define CAM_DIST_HPP

//standard headers
#include <map>
#include <tuple>
#include <variant>

//emdw headers
#include <sigmapoints.hpp>

//custom headers
#include <utils.hpp>

std::tuple<std::vector<gLinear::gColVector<double>>, std::vector<double>> getSigmaPoints(
	double w0,
	std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>> xi0,
	std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>> xj0,
	std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>> xi1,
	std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>> xj1
);
std::tuple<gLinear::gColVector<double>, gLinear::gRowMatrix<double>> transformSigmaPoints(std::vector<gLinear::gColVector<double>>sigmaPoints, std::vector<double> weights);
std::tuple<gLinear::gColVector<double>, gLinear::gRowMatrix<double>> getDisplacementLinearisedDistribution(
	double w0,
	std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>> xi0,
	std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>> xj0,
	std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>> xi1,
	std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>> xj1
);

#endif // !CAM_DIST_HPP
