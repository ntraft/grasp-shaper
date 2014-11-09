%% Normalize giant matrix

clear all

load('/Users/jolandefooken/Dropbox/termprojectsenmo/Analysis/logs/AllData/GripGTrain.mat')
load('/Users/jolandefooken/Dropbox/termprojectsenmo/Analysis/logs/AllData/GripGTest.mat')


load('/Users/jolandefooken/Dropbox/termprojectsenmo/Analysis/logs/AllData/GripMTrain.mat')
load('/Users/jolandefooken/Dropbox/termprojectsenmo/Analysis/logs/AllData/GripMTest.mat')


load('/Users/jolandefooken/Dropbox/termprojectsenmo/Analysis/logs/AllData/GripWTrain.mat')
load('/Users/jolandefooken/Dropbox/termprojectsenmo/Analysis/logs/AllData/GripWTest.mat')


GripGTrainNorm = featureNormalize(GripGTrainMatrix);
GripGTestNorm = featureNormalize(GripGTestMatrix);


save('GripGTrainNorm', 'GripGTrainNorm')
save('GripGTestNorm', 'GripGTestNorm')


GripMTrainNorm = featureNormalize(GripMTrainMatrix);
GripMTestNorm = featureNormalize(GripMTestMatrix);


save('GripMTrainNorm', 'GripMTrainNorm')
save('GripMTestNorm', 'GripMTestNorm')


GripWTrainNorm = featureNormalize(GripWTrainMatrix);
GripWTestNorm = featureNormalize(GripWTestMatrix);


save('GripWTrainNorm', 'GripWTrainNorm')
save('GripWTestNorm', 'GripWTestNorm')
