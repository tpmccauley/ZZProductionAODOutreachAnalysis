import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes

process = cms.Process("POET")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = "WARNING"
process.MessageLogger.categories.append("POET")
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    limit=cms.untracked.int32(-1))

process.options = cms.untracked.PSet(wantSummary=cms.untracked.bool(True))

goodJSON = 'data/Cert_13TeV_16Dec2015ReReco_Collisions15_25ns_JSON_v2.txt'
myLumis = LumiList.LumiList(filename = goodJSON).getCMSSWString().split(',') 

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = '76X_dataRun2_16Dec2015_v0'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

files = FileUtils.loadListFromFile('data/CMS_Run2015D_MuonEG_MINIAOD_16Dec2015-v1_60000_file_index.txt') 
readFiles = cms.untracked.vstring( *files )

process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring('root://eospublic.cern.ch//eos/opendata/cms/Run2015D/DoubleEG/MINIAOD/08Jun2016-v1/10000/00387F48-342F-E611-AB5D-0CC47A4D76AC.root')
    fileNames = readFiles
)

process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange()
process.source.lumisToProcess.extend(myLumis)

process.myelectrons = cms.EDAnalyzer('ElectronAnalyzer', electrons = cms.InputTag("slimmedElectrons"), 
                               vertices=cms.InputTag("offlineSlimmedPrimaryVertices"))
                              
process.mymuons = cms.EDAnalyzer('MuonAnalyzer', muons = cms.InputTag("slimmedMuons"), 
                               vertices=cms.InputTag("offlineSlimmedPrimaryVertices"))

process.mytriggers = cms.EDAnalyzer('TriggObjectAnalyzer', objects = cms.InputTag("selectedPatTrigger"))

process.mypvertex = cms.EDAnalyzer('VertexAnalyzer', 
	vertices=cms.InputTag("offlineSlimmedPrimaryVertices"),
	beams=cms.InputTag("offlineBeamSpot"))

process.TFileService = cms.Service("TFileService", fileName=cms.string("zzskim_output.root"))

process.zzskim = cms.EDFilter('ZZSkim',          
                              electronTag = cms.InputTag("slimmedElectrons"),
                              muonTag = cms.InputTag("slimmedMuons"),
                              vertexTag = cms.InputTag("offlineSlimmedPrimaryVertices")                             
                              )

process.p = cms.Path(
        process.zzskim+
	process.myelectrons+
	process.mymuons+
	process.mytriggers+
	process.mypvertex
	)

