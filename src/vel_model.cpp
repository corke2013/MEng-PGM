#include <vel_model.hpp>

typedef unsigned int AssignType;
typedef DiscreteTable<AssignType> DT;
enum { X = 1, M = 2, S = 3 };

// tested
VelModel::VelModel(std::vector<rcptr<Factor>>* factors, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>, double>> modelParameters) {
	VelModel::factors = factors;
	VelModel::modelParameters = modelParameters;
}

// tested
void VelModel::buildVelModel(unsigned int timeStep, std::map<unsigned int, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>> timeStepData) {
	rcptr<std::vector<AssignType>> binDom(new std::vector<AssignType>{ 0, 1 });
	for (auto& [featId, featData] : timeStepData) {
		std::size_t x = Utils::hashFromInts(X, timeStep, featId);
		std::size_t m = Utils::hashFromInts(M, timeStep, featId);
		gLinear::gColVector<double> velMean = std::get<gLinear::gColVector<double>>(featData["mean"]);
		gLinear::gRowMatrix<double> velCov = std::get<gLinear::gRowMatrix<double>>(featData["cov"]);
		double staticProb = Utils::getLikelyhood(velMean, velCov, std::get<gLinear::gColVector<double>>(modelParameters["staticMean"]), std::get<gLinear::gRowMatrix<double>>(modelParameters["staticCov"]));
		double dynamicProb = Utils::getLikelyhood(velMean, velCov, std::get<gLinear::gColVector<double>>(modelParameters["dynamicMean"]), std::get<gLinear::gRowMatrix<double>>(modelParameters["dynamicCov"]));

		// p(V(n)|X(n))
		factors->push_back(
			uniqptr<DT>(
				new DT(
					{
						x
					},
					{
						binDom
					},
					0.0,
					{
						{{0}, (staticProb / (staticProb + dynamicProb))},
						{{1}, (dynamicProb / (staticProb + dynamicProb))}
					}
		)));

		// p(X(n)|M(n))
		factors->push_back(
			uniqptr<DT>(
				new DT(
					{
						x,
						m
					},
					{
						binDom,
						binDom
					},
					0.0,
					{
						{{0, 0}, 1},
						{{0, 1}, 1 - std::get<double>(modelParameters["B"])},
						{{1, 0}, 1 - std::get<double>(modelParameters["B"])},
						{{1, 1}, std::get<double>(modelParameters["B"])}
					}
		)));
	}
}