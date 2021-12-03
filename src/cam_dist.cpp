#include <cam_dist.hpp>

std::tuple<std::vector<gLinear::gColVector<double>>, std::vector<double>> getSigmaPoints(
	double w0,
	std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>> xi0,
	std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>> xj0,
	std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>> xi1,
	std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>> xj1
) {
	std::vector<double> weights;
	std::vector<double> mean;
	std::vector<std::vector<std::vector<double>>> cov;
	std::vector<gLinear::gColVector<double>> sigmaPoints;
	std::vector<double> i0Mean = prlite::ColVector2vector(std::get<gLinear::gColVector<double>>(xi0["mean"]));
	std::vector<double> j0Mean = prlite::ColVector2vector(std::get<gLinear::gColVector<double>>(xj0["mean"]));
	std::vector<double> i1Mean = prlite::ColVector2vector(std::get<gLinear::gColVector<double>>(xi1["mean"]));
	std::vector<double> j1Mean = prlite::ColVector2vector(std::get<gLinear::gColVector<double>>(xj1["mean"]));
	std::vector<std::vector<double>> i0Cov = Utils::rowMatrixTo2DVector(std::get<gLinear::gRowMatrix<double>>(xi0["cov"]));
	std::vector<std::vector<double>> j0Cov = Utils::rowMatrixTo2DVector(std::get<gLinear::gRowMatrix<double>>(xj0["cov"]));
	std::vector<std::vector<double>> i1Cov = Utils::rowMatrixTo2DVector(std::get<gLinear::gRowMatrix<double>>(xi1["cov"]));
	std::vector<std::vector<double>> j1Cov = Utils::rowMatrixTo2DVector(std::get<gLinear::gRowMatrix<double>>(xj1["cov"]));

	mean.insert(mean.end(), i0Mean.begin(), i0Mean.end());
	mean.insert(mean.end(), j0Mean.begin(), j0Mean.end());
	mean.insert(mean.end(), i1Mean.begin(), i1Mean.end());
	mean.insert(mean.end(), j1Mean.begin(), j1Mean.end());

	cov.push_back(i0Cov);
	cov.push_back(j0Cov);
	cov.push_back(i1Cov);
	cov.push_back(j1Cov);

	int size = mean.size();
	gLinear::gColVector<double> startMean(prlite::vector2ColVector(mean));
	gLinear::gMatrix<double, gLinear::ROW_DENSE> startCov(size, size);
	startCov.assignToAll(0);

	for (unsigned long int i = 0; i < cov.size(); i++) {
		for (unsigned long int j = 0; j < cov[i].size(); j++) {
			for (unsigned long int k = 0; k < cov[i][j].size(); k++) {
				startCov[(i * cov[i].size()) +  j][(i * cov[i][j].size()) + k] = cov[i][j][k];
			}
		}
	}
	cov2sp_2Dp1(startMean, startCov, w0, sigmaPoints, weights);
	return std::make_tuple(sigmaPoints, weights);
}

std::tuple<gLinear::gColVector<double>, gLinear::gRowMatrix<double>> transformSigmaPoints(std::vector<gLinear::gColVector<double>>sigmaPoints, std::vector<double> weights){
	gLinear::gColVector<double> tranformMean;
	gLinear::gRowMatrix<double> transformCov;
	for (auto& sigmaPoint : sigmaPoints) {
		double r1 = Utils::getDistance(sigmaPoint[0], sigmaPoint[3], sigmaPoint[1], sigmaPoint[4], sigmaPoint[2], sigmaPoint[5]);
		double r2 = Utils::getDistance(sigmaPoint[6], sigmaPoint[9], sigmaPoint[7], sigmaPoint[10], sigmaPoint[8], sigmaPoint[11]);
		sigmaPoint.specialResize(1);
		sigmaPoint[0] = std::abs(r1 - r2);
	}
	sp2cov(sigmaPoints, weights, tranformMean, transformCov);
	return std::make_tuple(tranformMean, transformCov);
}


std::tuple<gLinear::gColVector<double>, gLinear::gRowMatrix<double>> getDisplacementLinearisedDistribution(
	double w0,
	std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>> xi0,
	std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>> xj0,
	std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>> xi1,
	std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>> xj1
) {
	std::tuple<std::vector<gLinear::gColVector<double>>, std::vector<double>> sigmaPoints = getSigmaPoints(w0, xi0, xj0, xi1, xj1);
	return transformSigmaPoints(std::get<0>(sigmaPoints), std::get<1>(sigmaPoints));
}