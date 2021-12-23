/* -------------------------------------------------------------------------- *
 * Copyright (c) 2010-2016 C. Pizzolato, M. Reggiani                          *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License");            *
 * you may not use this file except in compliance with the License.           *
 * You may obtain a copy of the License at:                                   *
 * http://www.apache.org/licenses/LICENSE-2.0                                 *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

#ifndef rtosim_JointContactFromOpensim_h
#define rtosim_JointContactFromOpensim_h

#include <string>
#include <vector>
#include <list>
#include <memory>
#include <OpenSim/OpenSim.h>
#include <OpenSim/Simulation/InverseDynamicsSolver.h>

namespace rtosim {

    class JointContactFromOpensim {
    public:
        JointContactFromOpensim(const std::string& modelFilename, std::vector<OpenSim::Force*> externalForces);
        virtual ~JointContactFromOpensim();
        void setMuscleForces(const std::vector<double>& forces);
        template<typename T>
        void setQ(const T& q);
        template<typename T>
        void setQd(const T& qd);
        template<typename T>
        void setQdd(const T& qdd);
        void setTime(double time);

        void calculateInverseDynamics();
        void calculateIntersegmentalForces();
        void calculateIntersegmentalForcesUsingExternalMobilityForces(const SimTK::Vector& realMobilityForces);
        void calculateJointReactionForces(const SimTK::Vector& muscleForces, const SimTK::Vector& estimatedMobilityForces);
        void calculateJointReactionForcesUsingExternalMobilityForces(const SimTK::Vector& muscleForces, const SimTK::Vector& estimatedMobilityForces, const SimTK::Vector& realMobilityForces);

        void getResidualMobilityForces(SimTK::Vector& residualMobilityForces) const;
        void getJointReactionForceInG(const std::string& jointName, SimTK::Vec3& forceOnChildInG) const;
        void getJointReactionForceInChild(const std::string& jointName, SimTK::Vec3& forceOnChildInChild) const;
        void getIntersegmentalForceInG(const std::string& jointName, SimTK::Vec3& forceOnChildInG) const;
        void getIntersegmentalForceInChild(const std::string& jointName, SimTK::Vec3& forceOnChildInChild) const;

        void getMuscleNamesFromOsimModel(std::vector<std::string>& names) const;
        void getBodyNamesFromOsimModel(std::vector<std::string>& names) const;
        void getCoordinateNamesFromOsimModel(std::vector<std::string>& names) const;
        void getJointNamesFromOsimModel(std::vector<std::string>& names) const;

        std::string getJointNameOnDof(const std::string& dofName);
        void getAllReactionForces(SimTK::Vector_<SimTK::Vec3>& reactionForces, SimTK::Vector_<SimTK::Vec3>& reactionTorques) const;
        SimTK::Vector getMobilityForces() const { return residualMobilityForces_; }

    private:
        void disableMuscleActuators();
        void enableMuscleActuators();
        void calculateReactions();
        void calculateReactions(const SimTK::Vector& residualMobilityForces);
        void setMuscleForcesToActuators(const SimTK::Vector& muscleForces);
        void calculateResidualMobilityForces(const SimTK::Vector& appliedMobilityForces);
        void getReactionForceInChild(const SimTK::Vector_<SimTK::Vec3>& rForcesInG, const std::string& jointName, SimTK::Vec3& forceOnChildInChild) const;
        void getReactionForceInG(const SimTK::Vector_<SimTK::Vec3>& rForcesInG, const std::string& jointName, SimTK::Vec3& forceOnChildInG) const;

        OpenSim::Model model_;
        SimTK::State state_;
        std::size_t nCoordinates_, nBodies_, nMuscles_;
        SimTK::Vector residualMobilityForces_;
        SimTK::Vector_<SimTK::Vec3> rForcesInG_, rTorquesInG_;
        SimTK::Vector_<SimTK::Vec3> intForcesInG_, jrForcesInG_;;
        SimTK::Vector_<SimTK::Real> qdd_;
        std::vector<size_t> OrderCIMTO_;
        std::vector<size_t> OrderSO_;

        std::vector<OpenSim::Muscle*> muscleActuators_;
        std::vector<OpenSim::CoordinateActuator*> coordinateActuators_;
        //need to use raw ptr because OpenSim
        OpenSim::InverseDynamicsSolver* idSolver_;
    };

    template<typename T>
    void JointContactFromOpensim::setQ(const T& q) {
        for (size_t i(0); i < nCoordinates_; ++i) {
            state_.updQ()[i] = q[i];
        }
    }

    template<typename T>
    void JointContactFromOpensim::setQd(const T& qd) {
        for (size_t i(0); i < nCoordinates_; ++i) {
            state_.updU()[i] = qd[i];
        }
    }

    template<typename T>
    void JointContactFromOpensim::setQdd(const T& qdd) {
        for (size_t i(0); i < nCoordinates_; ++i) {
            qdd_[i] = qdd[i];
        }
    }
}

#endif