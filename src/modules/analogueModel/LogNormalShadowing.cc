//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "LogNormalShadowing.h"

#include "Mapping.h"
#include "AirFrame_m.h"

LogNormalShadowing::LogNormalShadowing(double mean, double stdDev, simtime_t interval):
	mean(mean), stdDev(stdDev), interval(interval)
{ }

LogNormalShadowing::~LogNormalShadowing() {}

double LogNormalShadowing::randomLogNormalGain() const {
	return FWMath::dBm2mW(-1.0 * normal(mean, stdDev));
}

void LogNormalShadowing::filterSignal(AirFrame *frame) {
	Signal&   signal = frame->getSignal();
	simtime_t start  = signal.getReceptionStart();
	simtime_t end    = signal.getReceptionEnd();
	Mapping*  att    = MappingUtils::createMapping(DimensionSet::timeDomain, Mapping::LINEAR);

	Argument pos;

	for(simtime_t t = start; t <= end; t += interval)
	{
		pos.setTime(t);
		att->appendValue(pos, randomLogNormalGain());
	}

	signal.addAttenuation(att);
}