clear all

objectId = 50;
load('/Users/jolandefooken/Dropbox/termprojectsenmo/Analysis/logs/WristAttachement/dataLog13_g.mat')

% Find Index, when finger first close
toleranceUp = 5;
toleranceDown = 5;
TempMaximum = max(diff(BarryData.FingerTorques(:,3)));
TempMinimum = min(diff(BarryData.FingerTorques(:,3)));
GripStartIndex = find(diff(BarryData.FingerTorques(:,3))>TempMaximum-toleranceUp);
GripEndIndex = find(diff(BarryData.FingerTorques(:,3))<TempMinimum+toleranceDown);
%GripEndIndex = GripEndIndex(2);
GripStartEnd = GripEndIndex-GripStartIndex;

clear toleranceUp
clear toleranceDown

% Find Index, where it reached top position

tol = 0.005;
MinimalTemp = min(BarryData.WAMJointPositions(GripStartIndex:GripEndIndex,2));
IdxTemp = find(BarryData.WAMJointPositions(GripStartIndex:GripEndIndex,2)<MinimalTemp+tol);
ReachedGraspTopIndex = GripStartIndex + IdxTemp(1);

%Write everything into one Matrix
cd('/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis/logs/AllData')

load('/Users/jolandefooken/Dropbox/termprojectsenmo/Analysis/logs/AllData/GiganticMatrix.mat')
load('/Users/jolandefooken/Dropbox/termprojectsenmo/Analysis/logs/AllData/FeatureVector.mat')

MatrixDataTemp = [BarryData.HandJointPositions(GripStartIndex:GripEndIndex,:), ...
                  BarryData.FingerTorques(GripStartIndex:GripEndIndex,:),...
                  BarryData.WristForces(GripStartIndex:GripEndIndex,:),...
                  BarryData.TactileCellPressures.Sensor1(GripStartIndex:GripEndIndex,:),...
                  BarryData.TactileCellPressures.Sensor2(GripStartIndex:GripEndIndex,:),...
                  BarryData.TactileCellPressures.Sensor3(GripStartIndex:GripEndIndex,:),...
                  BarryData.TactileCellPressures.Sensor4(GripStartIndex:GripEndIndex,:)];
yTemp = objectId*ones(1,GripStartEnd+1)';

MatrixData = [MatrixData;MatrixDataTemp];
y = [y;yTemp];

save('GiganticMatrix', 'MatrixData')
save('FeatureVector', 'y')

cd ..
cd ..

clear all


