// patrec headers
#include <prlite_logging.hpp>
#include <prlite_testing.hpp>
#include <prlite_genmat.hpp>
#include <prlite_stlvecs.hpp>

// emdw headers
#include <emdw.hpp>
#include <discretetable.hpp>
#include <clustergraph.hpp>
#include <lbp_cg.hpp>
#include <lbu_cg.hpp>
#include <lbu2_cg.hpp>
#include <sigmapoints.hpp>

// standard headers
#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <memory>
#include <set>
#include <map>
#include <algorithm>
#include <limits>
#include <chrono>
#include <iomanip>

// custom headers
#include <cam_dist.hpp>
#include <preprocessing.hpp>
#include <json.hpp>
#include <vel_model.hpp>
#include <dis_model.hpp>

using namespace std;
using namespace emdw;
using json = nlohmann::json;

typedef unsigned int AssignType;
enum { X = 1, M = 2, S = 3 };

int main() {
	ifstream ifsIdxFeatIdVel("idx_feat_id_vel.json");
	//ifstream ifsIdxFeatIdCam("idx_feat_id_cam.json");
	json idxFeatIdVelJsonData = json::parse(ifsIdxFeatIdVel);
	//json idxFeatIdCamJsonData = json::parse(ifsIdxFeatIdCam);
	//double maxDistance = 3;
	Preprocessing preprocessing;

	std::map<unsigned int, std::map<unsigned int, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>>> idxFeatIdVelData = preprocessing.getIdxFeatIdVelData(idxFeatIdVelJsonData);
	//std::map<unsigned int, std::map<std::size_t, std::tuple<unsigned int, unsigned int, gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>> idxHashDisData = preprocessing.getIdxHashDisData(maxDistance, idxFeatIdCamJsonData);

	std::vector<rcptr<Factor>> factors;
	std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>, double>> modelParams{
		{"B", 0.5},
		{"staticMean", prlite::vector2ColVector(vector<double> {0, 0, 0})},
		{"staticCov", Utils::vector2DToRowMatrix(vector<vector<double>> { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}})},
		{"dynamicMean", prlite::vector2ColVector(vector<double> {0, 0, 0})},
		{"dynamicCov", Utils::vector2DToRowMatrix(vector<vector<double>> { {0.04, 0, 0}, {0, 0.04, 0}, {0, 0, 0.02}})}
	};
	VelModel velModel(&factors, modelParams);
	//DisModel disModel(&factors, modelParams);
	map<Idx2, rcptr<Factor>> messages;
	MessageQueue messageQueue;
	//std::vector<std::tuple<std::size_t, unsigned int, unsigned int>> rvIds;
	std::vector<rcptr<Factor>> newFactors;
	messages.clear();
	messageQueue.clear();
	//rvIds.clear();
	newFactors.clear();
	json output;
	std::ofstream file("key.json");

	for (unsigned int i = 0; i < 10; i++) {
		cerr << "Loop \n";
		if (idxFeatIdVelData.find(i) != idxFeatIdVelData.end()) {
			velModel.buildVelModel(i, idxFeatIdVelData[i]);
		}
		/*if (idxHashDisData.find(i) != idxHashDisData.end()) {
			disModel.buildDisModel(i, idxHashDisData[i], rvIds);
		}*/
		cerr << "Creating CG \n";
		ClusterGraph clusterGraph(factors);
		cerr << "Running LPB CG \n";
		loopyBP_CG(clusterGraph, messages, messageQueue, 0.0);
		newFactors.clear();

		// include information from this timestep to the next
		//cerr << "Extracting information for next time step \n";
		//for (auto rvId : rvIds) {
		//	std::size_t sij = std::get<0>(rvId);
		//	std::size_t sij1 = Utils::hashFromInts(S, i + 1, std::get<1>(rvId), std::get<2>(rvId));
		//	newFactors.push_back(rcptr<Factor>(queryLBP_CG(clusterGraph, messages, { sij })->copy({ sij1 })));
		//}

		// save results to file
		cerr << "Saving results to file \n";
		for (int j = 0; j < 600000; j++) {
			std::size_t mi = Utils::hashFromInts(X, i, j);
			try {
				output[std::to_string(i)][std::to_string(j)] = { {"cap_of_move", queryLBP_CG(clusterGraph, messages, { mi })->normalize()->potentialAt({ mi }, { AssignType(1) })} };
			}
			catch (...) {
			}
		}

		factors = newFactors;
		messages.clear();
		messageQueue.clear();
		//rvIds.clear();
	}
	file << output;
	usleep(1000000);
}