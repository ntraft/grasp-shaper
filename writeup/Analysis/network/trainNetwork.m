function [Theta1, Theta2, pred] = trainNetwork(X, y)
%TRAINNETWORK Train a two-layer neural network to perform classification.
%	Computes parameters for the input layer and hidden layer of the neural
%	network, and also performs prediction on the training set and returns
%	the result.
%

% clear; close all;
% load('/Users/ntraft/Dropbox/termprojectsenmo/Analysis/logs/AllData/FeatureG.mat');
% load('/Users/ntraft/Dropbox/termprojectsenmo/Analysis/logs/AllData/GetGripG.mat');
% X = GripGMatrix;
% y = g;


input_layer_size = size(X, 2);	% Number of features
hidden_layer_size = 25;   		% 25 hidden units
% num_labels = length(unique(y));	% Number of labels in y.
num_labels = 50;	% Number of labels in y.

fprintf('Initializing Neural Network Parameters ...\n')

initial_Theta1 = randInitializeWeights(input_layer_size, hidden_layer_size);
initial_Theta2 = randInitializeWeights(hidden_layer_size, num_labels);

% Unroll parameters
initial_nn_params = [initial_Theta1(:) ; initial_Theta2(:)];

fprintf('Training Neural Network... \n')

% Change to a larger value for more training.
options = optimset('MaxIter', 50);

% Can also try different regularization weights.
lambda = 1;

costFunction = @(p) nnCostFunction(p, ...
                                   input_layer_size, ...
                                   hidden_layer_size, ...
                                   num_labels, X, y, lambda);

% Now, costFunction is a function that takes in only one argument (the
% neural network parameters)
[nn_params, cost] = fmincg(costFunction, initial_nn_params, options);

% Obtain Theta1 and Theta2 back from nn_params
Theta1 = reshape(nn_params(1:hidden_layer_size * (input_layer_size + 1)), ...
                 hidden_layer_size, (input_layer_size + 1));

Theta2 = reshape(nn_params((1 + (hidden_layer_size * (input_layer_size + 1))):end), ...
                 num_labels, (hidden_layer_size + 1));

pred = predict(Theta1, Theta2, X);


fprintf('\nTraining Set Accuracy: %f\n', mean(double(pred == y)) * 100);

for cls = unique(y)'
	% cls
	truePos = sum((y == cls) & (pred == cls));
	fprintf('Precision of %d: %f\n', cls, truePos / sum(pred == cls));
	fprintf('Recall of %d: %f\n', cls, truePos / sum(y == cls));
end
