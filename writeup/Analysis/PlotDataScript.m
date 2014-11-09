%% Plotting script for Berry

% show what Logfile you are currently working on:
dataFileNameBarry

%% Plot the WAM Joint Positions

NoSubs = size(BarryData.WAMJointPositions,2);
greensHSV = [0.3333*ones(1, NoSubs)', 1*ones(1, NoSubs)', linspace(0.1, 1, NoSubs)'];
greens = hsv2rgb(greensHSV);
xWAM = (1:length(BarryData.WAMJointPositions))';

WAMJoints = figure(1);
for i=1:length(greens);
plot(xWAM, BarryData.WAMJointPositions(:,i),'Color', greens(i,:));
hold on
end
legend('Joint 1', 'Joint 2', 'Joint 3', 'Joint 4', 'Joint 5', 'Joint 6', 'Joint 7')
xlabel('Time [au]')
ylabel('WAM Joint Position [radians]')
hold off

cd('/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis/Figures/WAMJointPosPlots/')
hgsave(WAMJoints, dataFileNameBarry,'-v6')
cd('/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis/')

clear NoSubs
clear greensHSV
clear greens
clear i
clear WAMJoints

%% Plot the Hand Joint Positions
%load('/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis/logs/CookieForm/dataLog185_g.mat');

%darkgreen= [0,0.5,0];
turquoise = [0,0.75,0.75];
blue = [0,0,1];
darkblue = [0,0,0.5];
black = [0 0 0];
ColorV = [blue; turquoise; black];

% NoSubs = size(BarryData.HandJointPositions,2);
% redsHSV = [0.98*ones(1, NoSubs)', 1*ones(1, NoSubs)', linspace(0.1, 1, NoSubs)'];
% reds = hsv2rgb(redsHSV);
% 
 xWAM = (1:length(BarryData.HandJointPositions))';

HandJoints = figure(2);
for i=1:length(ColorV);
plot(xWAM/20, BarryData.HandJointPositions(:,i),'Color', ColorV(i,:));
hold on
end
legend('Joint Finger 1', 'Joint Finger 2', 'Joint Finger 3')
xlabel('Time [s]')
xlim([0 9.4])
ylabel('Hand Joint Position [radians]')
ylim([-0.2 1.2])
%title('Side-on Heavy Wrap of Cone')
hold off

%cd('/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis/Figures/HandJointPosPlots/')
%hgsave(HandJoints, dataFileNameBarry,'-v6')
%cd('/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis/')

clear NoSubs
clear redsHSV
clear reds
clear i
clear HandJoints

%% Plot the Finger Torqe values

darkgreen= [0,0.5,0];
turquoise = [0,0.75,0.75];
darkblue = [0,0,0.5];
ColorV = [darkgreen; turquoise; darkblue];

% NoSubs = size(BarryData.FingerTorques,2);
% bluesHSV = [0.57*ones(1, NoSubs)', 1*ones(1, NoSubs)', linspace(0.1, 1, NoSubs)'];
% blues = hsv2rgb(bluesHSV);
% xWAM = (1:length(BarryData.FingerTorques))';

FingerTorques = figure(3);
for i=1:ColorV;
plot(xWAM, BarryData.FingerTorques(:,i),'Color', ColorV(i,:));
hold on
end
legend('Torque Finger 1', 'Torque Finger 2', 'Torque Finger 3', 'Torque Spread Joint')
xlabel('Time [au]')
ylabel('Finger Torques [radians]')
hold off

%cd('/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis/Figures/FingerTorquesPlots/')
%hgsave(FingerTorques, dataFileNameBarry,'-v6')
%cd('/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis/')

clear NoSubs
clear bluesHSV
clear blues
clear i
clear FingerTorques

%% Plot the Wrist Forces

NoSubs = size(BarryData.WristForces,2);
purpelHSV = [0.75*ones(1, NoSubs)', 1*ones(1, NoSubs)', linspace(0.1, 1, NoSubs)'];
purpel = hsv2rgb(purpelHSV);
xWAM = (1:length(BarryData.WristForces))';

WristForces = figure(4);
for i=1:length(purpel);
plot(xWAM, BarryData.WristForces(:,i),'Color', purpel(i,:));
hold on
end
legend('Wrist Force 1', 'Wrist Force 2', 'Wrist Force 3')
xlabel('Time [au]')
ylabel('Wrist Forces [N]')
hold off

%cd('/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis/Figures/WristForcesPlots/')
%hgsave(WristForces, dataFileNameBarry,'-v6')
%cd('/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis/')

clear NoSubs
clear purpelHSV
clear purpel
clear i
clear WristForces

%% Plot the recorded Tactile Cell Pressures 1 Sensor in a subplot
load('/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis/logs/CookieForm/dataLog173_g.mat');


darkgreen= [0,0.5,0];
turquoise = [0,0.75,0.75];
darkblue = [0,0,0.5];
purple = [0.5,0,1];

% Maximal Pressure values 
n = size(BarryData.TactileCellPressures.Sensor1,2);

figure(6)
for i=1:n
    MaximalPressureSensor1(i) = max(BarryData.TactileCellPressures.Sensor1(:,i));
    MaximalPressureSensor2(i) = max(BarryData.TactileCellPressures.Sensor2(:,i));
    MaximalPressureSensor3(i) = max(BarryData.TactileCellPressures.Sensor3(:,i));
    MaximalPressureSensor4(i) = max(BarryData.TactileCellPressures.Sensor4(:,i));
end



subplot(221),plot(MaximalPressureSensor1,'Color', darkgreen),line([0 24], [0.5 0.5],'Color','r')
title('Finger 1'), xlabel('Sensor Cell #'), ylabel('Maximal Pressure [N/cm^2]')
subplot(222),plot(MaximalPressureSensor2,'Color', turquoise), line([0 24], [0.5 0.5],'Color','r')
title('Finger 2'), xlabel('Sensor Cell #'), ylabel('Maximal Pressure [N/cm^2]')
subplot(223),plot(MaximalPressureSensor3,'Color', darkblue), line([0 24], [0.5 0.5],'Color','r')
title('Finger 3'), xlabel('Sensor Cell #'), ylabel('Maximal Pressure [N/cm^2]')
subplot(224),plot(MaximalPressureSensor4, 'Color', purple), line([0 24], [0.5 0.5],'Color','r')
title('Palm'), xlabel('Sensor Cell #'), ylabel('Maximal Pressure [N/cm^2]')

clear i
clear n
clear darkgreen
clear turquoise
clear darkblue
clear purple

%%
%load('/Users/jolandefooken/Dropbox/TermProjectSenMo/Analysis/logs/CookieForm/dataLog172_g.mat');


% Use Tactile Data to create a map of maximal tactile response
n = size(BarryData.TactileCellPressures.Sensor1,2);

PositionSensor1 = ones(8,3);
PositionSensor2 = ones(8,3);
PositionSensor3 = ones(8,3);
PositionSensor4 = ones(8,3);


    MaximalPressureSensor1 = BarryData.TactileCellPressures.Sensor1(100,:);
    MaximalPressureSensor2 = BarryData.TactileCellPressures.Sensor2(100,:);
    MaximalPressureSensor3 = BarryData.TactileCellPressures.Sensor3(100,:);
    MaximalPressureSensor4 = BarryData.TactileCellPressures.Sensor4(100,:);

for j=3:3:24
    n = j/3;
    PositionSensor1(n,:) = MaximalPressureSensor1(1,j-2:j);
    PositionSensor2(n,:) = MaximalPressureSensor2(1,j-2:j);
    PositionSensor3(n,:) = MaximalPressureSensor3(1,j-2:j);
    PositionSensor4(n,:) = MaximalPressureSensor4(1,j-2:j);
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
