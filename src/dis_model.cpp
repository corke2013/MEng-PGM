#include <dis_model.hpp>

typedef std::size_t AssignType;
typedef DiscreteTable<AssignType> DT;
enum { X = 1, M = 2, S = 3, R = 4 };

DisModel::DisModel(std::vector<rcptr<Factor>>* factors, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>, double>> modelParameters) {
	DisModel::factors = factors;
	DisModel::modelParameters = modelParameters;
}

void DisModel::buildDisModel(unsigned int timeStep, std::map<std::size_t, std::tuple<unsigned int, unsigned int, prlite::ColVector<double>, prlite::RowMatrix<double>>> timeStepData) {
	rcptr< std::vector<AssignType> > binDom(new std::vector<AssignType>{ 0, 1 });

	for (auto [disId, disData] : timeStepData) {
		// p(Sij|Xi, Xj, Mi, Mj)
		factors->push_back(
			uniqptr<DT>(
				new DT(
					{
						Utils::hashFromInts(S, timeStep, std::get<0>(disData), std::get<1>(disData)),
						Utils::hashFromInts(X, timeStep, std::get<0>(disData)),
						Utils::hashFromInts(X, timeStep, std::get<1>(disData)),
						Utils::hashFromInts(M, timeStep, std::get<0>(disData)),
						Utils::hashFromInts(M, timeStep, std::get<1>(disData))
					},
					{
						binDom,
						binDom,
						binDom,
						binDom,
						binDom
					},
					0.0,
					{
						{{1, 0, 0, 0, 0}, 1},
						{{0, 0, 0, 1, 0}, 1},
						{{0, 0, 0, 0, 1}, 1},
						{{0, 1, 0, 1, 0}, 1},
						{{0, 0, 1, 0, 1}, 1},
						{{0, 1, 0, 1, 1}, 1},
						{{0, 0, 1, 1, 1}, 1},
						{{0, 0, 0, 1, 1}, 0.5},
						{{1, 0, 0, 1, 1}, 0.5},
						{{0, 1, 1, 1, 1}, 0.5},
						{{1, 1, 1, 1, 1}, 0.5},
						{{0, 1, 0, 0, 0}, 0.5},
						{{0, 1, 0, 0, 1}, 0.5},
						{{0, 1, 1, 0, 0}, 0.5},
						{{0, 1, 1, 0, 1}, 0.5},
						{{1, 1, 0, 0, 0}, 0.5},
						{{1, 1, 0, 0, 1}, 0.5},
						{{1, 1, 1, 0, 0}, 0.5},
						{{1, 1, 1, 0, 1}, 0.5},
						{{0, 0, 1, 0, 0}, 0.5},
						{{0, 0, 1, 1, 0}, 0.5},
						{{0, 1, 1, 0, 0}, 0.5},
						{{0, 1, 1, 1, 0}, 0.5},
						{{1, 0, 1, 0, 0}, 0.5},
						{{1, 0, 1, 1, 0}, 0.5},
						{{1, 1, 1, 0, 0}, 0.5},
						{{1, 1, 1, 1, 0}, 0.5},
					}
		)));

		// p(Rij|Sij)
		factors->push_back(
			uniqptr<DT>(
				new DT(
					{
						Utils::hashFromInts(S, timeStep, std::get<0>(disData), std::get<1>(disData))
					},
					{
						binDom
					},
					0.0,
					{
						{{0}, Utils::getLikelyhood(std::get<2>(disData), std::get<3>(disData), std::get<prlite::ColVector<double>>(modelParameters["dynamicMean"]), std::get<gLinear::gRowMatrix<double>>(modelParameters["dynamicCov"]))},
						{{1}, Utils::getLikelyhood(std::get<2>(disData), std::get<3>(disData), std::get<prlite::ColVector<double>>(modelParameters["staticMean"]), std::get<gLinear::gRowMatrix<double>>(modelParameters["staticCov"]))}
					}
		)));
	}
}