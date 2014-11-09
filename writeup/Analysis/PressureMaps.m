%% Sensor Map for Sensors

% show what Logfile you are currently working on:

% Use Tactile Data to create a map of maximal tactile response
n = size(BarryData.TactileCellPressures.Sensor1,2);

PositionSensor1 = ones(8,3);
PositionSensor2 = ones(8,3);
PositionSensor3 = ones(8,3);
PositionSensor4 = ones(8,3);

%for i=1:n
    MeanPressureSensor1 = mean(BarryData.TactileCellPressures.Sensor1);
    MeanPressureSensor2 = mean(BarryData.TactileCellPressures.Sensor2);
    MeanPressureSensor3 = mean(BarryData.TactileCellPressures.Sensor3);
    MeanPressureSensor4 = mean(BarryData.TactileCellPressures.Sensor4);
%end

for j=3:3:24
    n = j/3;
    PositionSensor1(n,:) = MeanPressureSensor1(:,j-2:j);
    PositionSensor2(n,:) = MeanPressureSensor2(:,j-2:j);
    PositionSensor3(n,:) = MeanPressureSensor3(:,j-2:j);
    PositionSensor4(n,:) = MeanPressureSensor4(:,j-2:j);
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
PressureMap = figure(1);

%title('Side-on Power Grip of Hard Foam Square')
subplot(221)
Colormap('Jet')
imagesc(PositionSensor1)
colorbar, caxis([0 10]) 
title('Finger 1')
axis off

subplot(222)
Colormap('Jet')
imagesc(PositionSensor2)
colorbar, caxis([0 10]) 
title('Finger 2')
axis off

subplot(223)
Colormap('Jet')
imagesc(PositionSensor3)
colorbar, caxis([0 10]) 
title('Finger 3')
axis off

subplot(224)
Colormap('Jet')
imagesc(PositionSensor4)
colorbar, caxis([0 10]) 
title('Palm')
axis off


%save data in TactileFeedbackMaps
% cd('/Users/jolandefooken/Dropbox/termprojectsenmo/Analysis/Figures/TactileFeedbackMaps/')
% hgsave(PressureMap, dataFileNameBarry,'-v6')
% cd('/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis/')

%clear all
