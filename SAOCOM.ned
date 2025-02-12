import ned.DatarateChannel;
import ned.IUnidirectionalChannel;


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
// 
// TODO documentation
//
simple GroundStation
{
    @display("i=device/receiverdish");
    @signal[contact](type=double);
    gates:
        output out;
}

//
// TODO documentation
//
simple USS
{
    @display("i=device/lan-bus");
    gates:
        input in;
}

network SAOCOMNetwork
{
    @display("bgb=316.9231,261.53848");
    types:
        channel GroundNetworkChannel extends DatarateChannel{
            parameters:
                int sim_start_time_year = 2019;
        		int sim_start_time_month = 2;
        		int sim_start_time_day = 23;
        	@class(GroundNetworkChannel);
        	@signal[usageTime](type=double);
        	@statistic[usageTime](title="Network Usage Time"; record=vector);
        	};
        
    submodules:
        GS: GroundStation;
        USS: USS {
            @display("p=256.15387,130");
        }
    connections:
        GS.out --> GroundNetworkChannel { datarate = 4Mbps;} --> USS.in;
}
