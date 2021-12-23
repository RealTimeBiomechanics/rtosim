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

#include "rtosim/JointContactFromOpensim.h"
#include "rtosim/ArrayConverter.h"
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;

namespace rtosim {

    JointContactFromOpensim::JointContactFromOpensim(const std::string& modelFilename, std::vector<OpenSim::Force*> externalForces) :
        model_(modelFilename),
        idSolver_(nullptr)
    {

        OpenSim::Model modelCopy(modelFilename);
        modelCopy.initSystem();
        nCoordinates_ = modelCopy.getNumCoordinates();
        nBodies_ = modelCopy.getNumBodies();
        nMuscles_ = modelCopy.getMuscles().getSize();
        rForcesInG_.resize(nBodies_);
        rTorquesInG_.resize(nBodies_);

        qdd_.resize(nCoordinates_);

        //1. add extForces to the model
        for (auto &force : externalForces) {
            model_.addForce(force);
        }

        idSolver_ = new OpenSim::InverseDynamicsSolver(model_);

        state_ = model_.initSystem();

        OpenSim::Set<OpenSim::Actuator>& acts = model_.updActuators();
        for (int i = 0; i < acts.getSize(); i++) {
            acts[i].setAppliesForce(state_, false);
        }
        OpenSim::Set<OpenSim::Muscle>& muscles = model_.updMuscles();
        for (int i = 0; i < muscles.getSize(); i++) {
            muscles[i].setAppliesForce(state_, false);
        }


        std::vector<std::string> coordNamesCIMTO, coordNamesOSSTD;
        auto coords = model_.getCoordinatesInMultibodyTreeOrder();
        for (auto& it : coords) {
            coordNamesCIMTO.push_back(it->getName());
        }

        OpenSim::Array<std::string> coordNamesOS;
        model_.getCoordinateSet().getNames(coordNamesOS);


        for (int i = 0; i < coordNamesOS.size(); i++)
        {
            coordNamesOSSTD.push_back(coordNamesOS[i]);
            auto result = std::find(coordNamesCIMTO.begin(), coordNamesCIMTO.end(), coordNamesOS[i]);
            OrderCIMTO_.push_back(std::distance(coordNamesCIMTO.begin(), result));
        }
        std::cout << std::endl;

        for (int i = 0; i < coordNamesCIMTO.size(); i++)
        {
            auto result = std::find(coordNamesOSSTD.begin(), coordNamesOSSTD.end(), coordNamesCIMTO[i]);
            OrderSO_.push_back(std::distance(coordNamesOSSTD.begin(), result));
        }
    }

    void JointContactFromOpensim::setTime(double time) {

        state_.updTime() = time;
    }

    void JointContactFromOpensim::getResidualMobilityForces(SimTK::Vector& residualMobilityForces) const {

        residualMobilityForces = residualMobilityForces_;
    }

    void JointContactFromOpensim::getMuscleNamesFromOsimModel(vector<string>& names) const {

        OpenSim::Array<string> namesOsim;
        model_.getMuscles().getNames(namesOsim);
        ArrayConverter::toStdVector(namesOsim, names);
    }

    void JointContactFromOpensim::getCoordinateNamesFromOsimModel(vector<string>& names) const {

        OpenSim::Array<string> namesOsim;
        model_.getCoordinateSet().getNames(namesOsim);
        ArrayConverter::toStdVector(namesOsim, names);
    }

    void JointContactFromOpensim::getBodyNamesFromOsimModel(vector<string>& names) const {

        OpenSim::Array<string> namesOsim;
        model_.getBodySet().getNames(namesOsim);
        ArrayConverter::toStdVector(namesOsim, names);
    }

    void JointContactFromOpensim::getJointNamesFromOsimModel(vector<string>& names) const {

        OpenSim::Array<string> namesOsim;
        model_.getJointSet().getNames(namesOsim);
        ArrayConverter::toStdVector(namesOsim, names);
    }

    void JointContactFromOpensim::getJointReactionForceInG(const string& jointName, SimTK::Vec3& forceOnChildInG) const {

        getReactionForceInG(jrForcesInG_, jointName, forceOnChildInG);
    }

    void JointContactFromOpensim::getJointReactionForceInChild(const string& jointName, SimTK::Vec3& forceOnChildInChild) const {

        getReactionForceInChild(jrForcesInG_, jointName, forceOnChildInChild);
    }

    void JointContactFromOpensim::getIntersegmentalForceInG(const string& jointName, SimTK::Vec3& forceOnChildInG) const {

        getReactionForceInG(intForcesInG_, jointName, forceOnChildInG);
    }

    void JointContactFromOpensim::getIntersegmentalForceInChild(const string& jointName, SimTK::Vec3& forceOnChildInChild) const {

        getReactionForceInChild(intForcesInG_, jointName, forceOnChildInChild);
    }

    void JointContactFromOpensim::getReactionForceInChild(const SimTK::Vector_<SimTK::Vec3>& rForcesInG, const std::string& jointName, SimTK::Vec3& forceOnChildInChild) const {

        const OpenSim::Joint& joint(model_.getJointSet().get(jointName));
        unsigned idx(model_.getJointSet().getIndex(joint.getName()));
        model_.getSimbodyEngine().transform(state_,
            model_.getGround(),
            rForcesInG.get(idx),
            joint.getParentFrame(),
            forceOnChildInChild);
    }

    void JointContactFromOpensim::getReactionForceInG(const SimTK::Vector_<SimTK::Vec3>& rForcesInG, const std::string& jointName, SimTK::Vec3& forceOnChildInG)  const {

        const OpenSim::Joint& joint(model_.getJointSet().get(jointName));
        unsigned idx(model_.getJointSet().getIndex(joint.getName()));
        //for some reason everything is shifted by 1.. this is a temporary patch.. to find a more robust solution
        forceOnChildInG = rForcesInG.get(idx + 1);
    }

    string JointContactFromOpensim::getJointNameOnDof(const string& dofName) {

        if (model_.getCoordinateSet().getIndex(dofName) < 0) {
            cout << "Error " + dofName + " not found\n";
            std::exit(EXIT_FAILURE);
        }
        return model_.getCoordinateSet().get(dofName).getJoint().getName();
    }

    void JointContactFromOpensim::calculateIntersegmentalForces() {

        calculateInverseDynamics();
        disableMuscleActuators();
        calculateReactions();
        intForcesInG_ = rForcesInG_;
    }

    void JointContactFromOpensim::calculateIntersegmentalForcesUsingExternalMobilityForces(const SimTK::Vector& realMobilityForces) {

        residualMobilityForces_ = realMobilityForces;
        disableMuscleActuators();
        calculateReactions();
        intForcesInG_ = rForcesInG_;
    }

    void JointContactFromOpensim::calculateJointReactionForces(const SimTK::Vector& muscleForces, const SimTK::Vector& estimatedMobilityForces) {

        calculateInverseDynamics();
        enableMuscleActuators();
        setMuscleForcesToActuators(muscleForces);
        SimTK::Vector balancingResidual(residualMobilityForces_ - estimatedMobilityForces);
        calculateReactions(balancingResidual);
        jrForcesInG_ = rForcesInG_;
    }

    void JointContactFromOpensim::calculateJointReactionForcesUsingExternalMobilityForces(const SimTK::Vector& muscleForces, const SimTK::Vector& estimatedMobilityForces, const SimTK::Vector& realMobilityForces) {

        enableMuscleActuators();
        setMuscleForcesToActuators(muscleForces);
        calculateReactions(realMobilityForces - estimatedMobilityForces);
        jrForcesInG_ = rForcesInG_;
    }

    void JointContactFromOpensim::calculateInverseDynamics() {
        //this updates residualMobilityForces_
        disableMuscleActuators();
        SimTK::Vector appliedMobilityForces(nCoordinates_, 0.); //Calculate ID
        calculateResidualMobilityForces(appliedMobilityForces);
    }

    void JointContactFromOpensim::disableMuscleActuators() {

        for (auto& m : muscleActuators_)
            m->setAppliesForce(state_, false);
    }

    void JointContactFromOpensim::enableMuscleActuators() {

        for (auto& m : muscleActuators_)
            m->setAppliesForce(state_, true);
    }

    void JointContactFromOpensim::setMuscleForcesToActuators(const SimTK::Vector& muscleForces) {

        for (size_t i(0); i < muscleActuators_.size(); ++i) {
            muscleActuators_.at(i)->setOverrideActuation(state_, true);
            muscleActuators_.at(i)->setOverrideActuation(state_, muscleForces[i]);
        }
    }

    void JointContactFromOpensim::calculateReactions() {

        calculateReactions(residualMobilityForces_);
    }

    void JointContactFromOpensim::calculateReactions(const SimTK::Vector& residualMobilityForces) {

        for (unsigned i(0); i < nCoordinates_; ++i) {
            coordinateActuators_.at(i)->overrideActuation(state_, true);
            coordinateActuators_.at(i)->setOverrideActuation(state_, residualMobilityForces.get(i));
        }
        model_.getMultibodySystem().realize(state_, SimTK::Stage::Acceleration);
        model_.getSimbodyEngine().computeReactions(state_, rForcesInG_, rTorquesInG_);
    }

    //if appliedMobilityForces is 0 then it is the ID
    void JointContactFromOpensim::calculateResidualMobilityForces(const SimTK::Vector& appliedMobilityForces) {

        //realize the system, so all the forces as gravitational, inertial, and external forces are applied
        model_.getMultibodySystem().realize(state_, SimTK::Stage::Dynamics);
        SimTK::Vector_<SimTK::SpatialVec> appliedBodyForces(model_.getMultibodySystem().getRigidBodyForces(state_, SimTK::Stage::Dynamics));
        residualMobilityForces_ = idSolver_->solve(state_, qdd_, appliedMobilityForces, appliedBodyForces);
    }

    void JointContactFromOpensim::getAllReactionForces(SimTK::Vector_<SimTK::Vec3>& reactionForces, SimTK::Vector_<SimTK::Vec3>& reactionTorques) const {
        reactionForces = rForcesInG_;
        reactionTorques = rTorquesInG_;
    }

    JointContactFromOpensim::~JointContactFromOpensim() {

        //     if (idSolver_ != nullptr)
        //         delete idSolver_;
    }
}