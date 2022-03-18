#include <dis_model.hpp>

typedef unsigned int AssignType;
typedef DiscreteTable<AssignType> DT;
enum { X = 1, M = 2, S = 3 };

DisModel::DisModel(std::vector<rcptr<Factor>>* factors, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>, double>> modelParameters) {
	DisModel::factors = factors;
	DisModel::modelParameters = modelParameters;
}

void DisModel::buildDisModel(unsigned int timeStep, std::map<std::size_t, std::tuple<unsigned int, unsigned int, prlite::ColVector<double>, prlite::RowMatrix<double>>> timeStepData, std::vector<std::tuple<std::size_t, unsigned int, unsigned int>>& rvIds) {
	rcptr< std::vector<AssignType> > binDom(new std::vector<AssignType>{ 0, 1 });
	for (auto& [disId, disData] : timeStepData) {
		std::size_t sij = Utils::hashFromInts(S, timeStep, std::get<0>(disData), std::get<1>(disData));
		std::size_t xi = Utils::hashFromInts(X, timeStep, std::get<0>(disData));
		std::size_t xj = Utils::hashFromInts(X, timeStep, std::get<1>(disData));
		std::size_t mi = Utils::hashFromInts(M, timeStep, std::get<0>(disData));
		std::size_t mj = Utils::hashFromInts(M, timeStep, std::get<1>(disData));
		rvIds.push_back(std::make_tuple(sij, std::get<0>(disData), std::get<1>(disData)));

		// p(Sij|Xi, Xj, Mi, Mj)
		factors->push_back(
			uniqptr<DT>(
				new DT(
					{
						sij,
						xi,
						xj,
						mi,
						mj
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
						sij
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