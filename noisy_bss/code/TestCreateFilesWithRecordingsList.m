function []=TestCreateFilesWithRecordingsList()

clear all;clc

speakersPath = '/Users/zz4fap/Desktop/BSS+ASR/data_base/speech/todos/treino/wav/';
fs=8000;
numberOfRecordings=1200;
numberOfFilesToCreate=10;

CreateFilesWithRecordingsList(numberOfRecordings,numberOfFilesToCreate,fs,speakersPath);