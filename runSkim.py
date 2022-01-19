import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList
from FWCore.MessageLogger.MessageLogger_cfi import *

process = cms.Process("ZZPROD")

goodJSON = 'data/Cert_13TeV_16Dec2015ReReco_Collisions15_25ns_JSON_v2.txt'
myLumis = LumiList.LumiList(filename = goodJSON).getCMSSWString().split(',') 

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = '76X_dataRun2_16Dec2015_v0'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'root://eospublic.cern.ch//eos/opendata/cms/Run2015D/DoubleEG/MINIAOD/08Jun2016-v1/10000/00387F48-342F-E611-AB5D-0CC47A4D76AC.root',
#'root://eospublic.cern.ch//eos/opendata/cms/Run2015D/DoubleEG/MINIAOD/08Jun2016-v1/10000/0087F76E-F52F-E611-BEED-002590D0AFA0.root',
#'root://eospublic.cern.ch//eos/opendata/cms/Run2015D/DoubleEG/MINIAOD/08Jun2016-v1/10000/00A3A3A7-182F-E611-860C-0CC47A78A426.root',
#'root://eospublic.cern.ch//eos/opendata/cms/Run2015D/DoubleEG/MINIAOD/08Jun2016-v1/10000/00BF6041-5F2F-E611-9FCA-B083FED76D99.root',
#'root://eospublic.cern.ch//eos/opendata/cms/Run2015D/DoubleEG/MINIAOD/08Jun2016-v1/10000/00F0FC81-D82F-E611-AC3B-0CC47A4C8ECE.root',
#'root://eospublic.cern.ch//eos/opendata/cms/Run2015D/DoubleEG/MINIAOD/08Jun2016-v1/10000/020DC28C-CB2F-E611-B9F7-0CC47A4D76BE.root',
#'root://eospublic.cern.ch//eos/opendata/cms/Run2015D/DoubleEG/MINIAOD/08Jun2016-v1/10000/0263DCD4-542F-E611-9412-0025905C5438.root',
#'root://eospublic.cern.ch//eos/opendata/cms/Run2015D/DoubleEG/MINIAOD/08Jun2016-v1/10000/0278E117-C52F-E611-9C80-0025905B855C.root',
#'root://eospublic.cern.ch//eos/opendata/cms/Run2015D/DoubleEG/MINIAOD/08Jun2016-v1/10000/02904D4E-D82F-E611-8AC5-0CC47A4D75EE.root',
#'root://eospublic.cern.ch//eos/opendata/cms/Run2015D/DoubleEG/MINIAOD/08Jun2016-v1/10000/02A8BB7A-BC2F-E611-9ECE-7845C4FC3B7B.root',
#'root://eospublic.cern.ch//eos/opendata/cms/Run2015D/DoubleEG/MINIAOD/08Jun2016-v1/10000/02BE2A54-E62E-E611-B129-008CFA1982C0.root',


 )
)

process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange()
process.source.lumisToProcess.extend(myLumis)

process.zzskim = cms.EDFilter('ZZSkim',          
                              electronTag = cms.InputTag("slimmedElectrons"),
                              muonTag = cms.InputTag("slimmedMuons"),
                              vertexTag = cms.InputTag("offlineSlimmedPrimaryVertices")                             
                              )


process.zzprod = cms.Path(process.zzskim)
process.schedule = cms.Schedule(process.zzprod)
