import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList
from FWCore.MessageLogger.MessageLogger_cfi import *

process = cms.Process("ZZPROD")

goodJSON = 'data/Cert_13TeV_16Dec2015ReReco_Collisions15_25ns_JSON_v2.txt'
myLumis = LumiList.LumiList(filename = goodJSON).getCMSSWString().split(',') 

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = '76X_dataRun2_16Dec2015_v0'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
 'root://eospublic.cern.ch//eos/opendata/cms/Run2015D/DoubleEG/MINIAOD/08Jun2016-v1/10000/00387F48-342F-E611-AB5D-0CC47A4D76AC.root'
 )
)

process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange()
process.source.lumisToProcess.extend(myLumis)

process.zzskim = cms.EDFilter('ZZSkim',          
                              electronTag = cms.InputTag("slimmedElectrons"),
                              muonTag = cms.InputTag("slimmedMuons")                             
                              )


process.zzprod = cms.Path(process.zzskim)
process.schedule = cms.Schedule(process.zzprod)
