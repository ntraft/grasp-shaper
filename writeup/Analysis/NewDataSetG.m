%% Do as much as you can in one script (sorry Neil, I know you don't like
%% this...

%% Data parsing Barry

close all
clear all

cd('/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis/logs/Fail')
defaultFolder = '/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis/logs/Fail';
objectId = 17;

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

dataFileNameBarry
save(dataFileNameBarry, 'BarryData')

% Use Tactile Data to create a map of maximal tactile response
n = size(BarryData.TactileCellPressures.Sensor1,2);

PositionSensor1 = ones(8,3);
PositionSensor2 = ones(8,3);
PositionSensor3 = ones(8,3);
PositionSensor4 = ones(8,3);

for i=1:n
    MaximalPressureSensor1(i) = max(BarryData.TactileCellPressures.Sensor1(:,i));
    MaximalPressureSensor2(i) = max(BarryData.TactileCellPressures.Sensor2(:,i));
    MaximalPressureSensor3(i) = max(BarryData.TactileCellPressures.Sensor3(:,i));
    MaximalPressureSensor4(i) = max(BarryData.TactileCellPressures.Sensor4(:,i));
end

for j=3:3:24
    n = j/3;
    PositionSensor1(n,:) = MaximalPressureSensor1(:,j-2:j);
    PositionSensor2(n,:) = MaximalPressureSensor2(:,j-2:j);
    PositionSensor3(n,:) = MaximalPressureSensor3(:,j-2:j);
    PositionSensor4(n,:) = MaximalPressureSensor4(:,j-2:j);
end

clear i
clear j
clear n

% We want to have the finger tip on the top
PositionSensor1 = flipud(PositionSensor1);
PositionSensor2 = flipud(PositionSensor2);
PositionSensor3 = flipud(PositionSensor3);
PositionSensor4 = flipud(PositionSensor4);


% Plot Pressure Maps for 4 different sensor arrays
PressureMap = figure(100);

subplot(221)
Colormap('Jet')
imagesc(PositionSensor1)
colorbar, caxis([0 16]) 
title('Finger 1')
axis off

subplot(222)
Colormap('Jet')
imagesc(PositionSensor2)
colorbar, caxis([0 16]) 
title('Finger 2')
axis off

subplot(223)
Colormap('Jet')
imagesc(PositionSensor3)
colorbar, caxis([0 16]) 
title('Finger 3')
axis off

subplot(224)
Colormap('Jet')
imagesc(PositionSensor4)
colorbar, caxis([0 16]) 
title('Palm')
axis off

%save data in TactileFeedbackMaps
hgsave(PressureMap, dataFileNameBarry,'-v6')

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

%%


% Find Index, when finger first close
toleranceUp = 0.0005;
toleranceDown = 0.0005;
%toleranceUp = 0.01;
%toleranceDown = 0.001;
TempMaximum = max(diff(BarryData.HandJointPositions(:,1)));
TempMinimum = min(diff(BarryData.HandJointPositions(10:end,1)));
GripStartIndex = find(diff(BarryData.HandJointPositions(:,1))>TempMaximum-toleranceUp)+3
GripStartIndex = GripStartIndex(1);
GripEndIndex = find(diff(BarryData.HandJointPositions(:,1))<TempMinimum+toleranceDown)-3
GripEndIndex = GripEndIndex(1);
GripStartEnd = GripEndIndex-GripStartIndex;

clear toleranceUp
clear toleranceDown


%Write everything into one Matrix
cd('/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis/logs/AllData')

load('/Users/jolandefooken/Dropbox/termprojectsenmo/Analysis/logs/AllData/GiganticMatrixG.mat')
load('/Users/jolandefooken/Dropbox/termprojectsenmo/Analysis/logs/AllData/LabelVectorG.mat')

MatrixDataTemp = [BarryData.HandJointPositions(GripStartIndex:GripEndIndex,:), ...
                  BarryData.WristForces(GripStartIndex:GripEndIndex,:),...
                  BarryData.TactileCellPressures.Sensor1(GripStartIndex:GripEndIndex,:),...
                  BarryData.TactileCellPressures.Sensor2(GripStartIndex:GripEndIndex,:),...
                  BarryData.TactileCellPressures.Sensor3(GripStartIndex:GripEndIndex,:),...
                  BarryData.TactileCellPressures.Sensor4(GripStartIndex:GripEndIndex,:)];
yTemp = objectId*ones(1,GripStartEnd+1)';

MatrixData = [MatrixData;MatrixDataTemp];
y = [y;yTemp];

save('GiganticMatrixG', 'MatrixData')
save('LabelVectorG', 'y')


%%

cd('/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis')