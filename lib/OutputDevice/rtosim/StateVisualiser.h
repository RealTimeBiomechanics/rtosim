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

#ifndef rtosim_StateVisualiser_h
#define rtosim_StateVisualiser_h

#include <string>
#include <OpenSim/OpenSim.h>
#include "rtosim/concurrency/Concurrency.h"
#include "rtosim/queue/GeneralisedCoordinatesQueue.h"

namespace rtosim{

    class ShowInfo : public SimTK::DecorationGenerator {
    public:
        ShowInfo() : text_(""){}
        void setText(const std::string& text) {
            text_ = text;
        }
        void generateDecorations(const SimTK::State& state,
            SimTK::Array_<SimTK::DecorativeGeometry>& geometry) override
        {
            SimTK::DecorativeText info;
            info.setIsScreenText(true);
            info.setText(text_);
            geometry.push_back(info);
        }

    private:
        std::string text_;
    };

    class StateVisualiser {
    public:
        StateVisualiser(
            rtosim::GeneralisedCoordinatesQueue& inputGeneralisedCoordinatesQueue,
            const std::string& osimModelFilename);
        StateVisualiser(StateVisualiser&) = delete;
        virtual ~StateVisualiser() {}
        void operator()();
    private:
        OpenSim::Model model_;
        rtosim::GeneralisedCoordinatesQueue& inputGeneralisedCoordinatesQueue_;
    };
}
#endif
