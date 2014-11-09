%% Data parsing Barry

close all
clear all

cd('/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis/logs/CookieForm')
defaultFolder = '/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis/logs/CookieForm';

if exist('PathName')

if isnumeric(PathName);
    
    % Default folder
    folder = defaultFolder;
    
else
    % Open folder that contained the last processed file
    folder = PathName;
    oldFolder = PathName;
    
end

else
    % Default folder
    folder = defaultFolder;
    
end

% Get Pahntom data file
[FileNameBarry,PathName,FilterIndex] = uigetfile('*.log','Select Barrys data file',folder);
if FilterIndex == 0
    PathName = folder;
    break
end

dataFileNameBarry = strrep(FileNameBarry, '.log',''); %name of the file, used later
Name = strcat(PathName, FileNameBarry);
BarryDataTemp = load(Name);

BarryData = struct('Time',BarryDataTemp(:,1),'WAMJointPositions',BarryDataTemp(:,2:8),'HandJointPositions',BarryDataTemp(:,9:12),... 
                   'WristForces',BarryDataTemp(:,13:15),'TactileCellPressuresTemp',BarryDataTemp(:,16:111));
BarryData.TactileCellPressures = struct('Sensor1',BarryData.TactileCellPressuresTemp(:,1:24),...
                                        'Sensor2',BarryData.TactileCellPressuresTemp(:,25:48),...
                                        'Sensor3',BarryData.TactileCellPressuresTemp(:,49:72),...
                                        'Sensor4',BarryData.TactileCellPressuresTemp(:,73:96));
                                    
BarryData = rmfield(BarryData,'TactileCellPressuresTemp'); 


save(dataFileNameBarry, 'BarryData')

clear FileNameBarry
clear BarryDataTemp
clear BarryData.TactileCellPressuresTemp
clear FilterIndex
clear Name
clear PathName
clear file
clear folder
clear oldFolder
clear FileName

cd('/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis/')