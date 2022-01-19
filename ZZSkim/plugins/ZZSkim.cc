#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

class ZZSkim : public edm::stream::EDFilter<> {
   public:
      explicit ZZSkim(const edm::ParameterSet&);
      ~ZZSkim();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginStream(edm::StreamID) override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endStream() override;

      edm::InputTag electronInputTag_;
      edm::InputTag muonInputTag_;
      edm::InputTag vertexInputTag_;
 
      edm::EDGetTokenT<std::vector<pat::Electron> > electronToken_;
      edm::EDGetTokenT<std::vector<pat::Muon> > muonToken_;
      edm::EDGetTokenT<reco::VertexCollection> vertexToken_;

};

ZZSkim::ZZSkim(const edm::ParameterSet& iConfig) :
electronInputTag_(iConfig.getParameter<edm::InputTag>("electronTag")),
muonInputTag_(iConfig.getParameter<edm::InputTag>("muonTag")),
vertexInputTag_(iConfig.getParameter<edm::InputTag>("vertexTag"))
{
        electronToken_ = consumes<std::vector<pat::Electron> >(electronInputTag_);
	muonToken_ = consumes<std::vector<pat::Muon> >(muonInputTag_);
        vertexToken_ = consumes<reco::VertexCollection>(vertexInputTag_);
}

ZZSkim::~ZZSkim()
{}

bool
ZZSkim::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   edm::Handle<reco::VertexCollection> vertices;
   iEvent.getByToken(vertexToken_, vertices);

   if ( vertices->empty() ) {
     return false;
   }

   const reco::Vertex &pv = vertices->front();
   
   edm::Handle<std::vector<pat::Electron> > electrons;
   iEvent.getByToken(electronToken_, electrons);
 
   edm::Handle<std::vector<pat::Muon> > muons;
   iEvent.getByToken(muonToken_, muons);

   unsigned int nElectron = 0;
   unsigned int nMuon = 0;

   nElectron = electrons->size();
   nMuon = muons->size();

   // Reject if fewer than 4 leptons
   if ( (nElectron+nMuon) < 4 ) {
	return false;
   }

   // We want at least one lepton to have a pt > 20 GeV
   // and for next one to have a pt > 10 GeV
   bool pt_gt20 = false;
   bool pt_gt10 = false;	

   std::vector<pat::Electron> my_electrons;
   std::vector<pat::Muon> my_muons;

   for ( const pat::Electron &e: *electrons )
    {   
       auto pt = e.pt();

	if ( pt < 7 || 
             fabs(e.eta()) > 2.5 ) {
          continue;
        }

        auto dxy = e.bestTrack()->dxy(pv.position());
	auto dz = e.bestTrack()->dz(pv.position());
        auto dxyErr = e.bestTrack()->dxyError();
        auto dzErr = e.bestTrack()->dzError();

	auto sip3d = sqrt(dxy*dxy + dz*dz);
        sip3d /= sqrt(dxyErr*dxyErr + dzErr*dzErr);

        if ( fabs(dxy) > 0.5 || fabs(dz) > 1.0 || sip3d > 4 ) {
          continue;
        }

	auto iso = e.ecalPFClusterIso();

	if ( iso/pt > 0.35 ) {
          continue;
        }

	if ( pt > 20 ) {
	  pt_gt20 = true;
        } 

	if ( pt > 10 && pt < 20 ) {
          pt_gt10 = true;
        }

        my_electrons.push_back(e);

    }

    for ( const pat::Muon &m: *muons )
    {   
        auto pt = m.pt();

        if ( pt < 5 || 
             fabs(m.eta()) > 2.4 ) {
          continue;
        }

        auto dxy = m.bestTrack()->dxy(pv.position());
        auto dz = m.bestTrack()->dz(pv.position());
        auto dxyErr = m.bestTrack()->dxyError();
        auto dzErr = m.bestTrack()->dzError();

        auto sip3d = sqrt(dxy*dxy + dz*dz);
        sip3d /= sqrt(dxyErr*dxyErr + dzErr*dzErr);

        if ( fabs(dxy) > 0.5 || fabs(dz) > 1.0 || sip3d > 4 ) {
          continue;
        }

	auto iso03 = m.pfIsolationR03();
        //auto iso04 = m.pfIsolationR04();

        auto relIso03 = (iso03.sumChargedHadronPt + iso03.sumNeutralHadronEt + iso03.sumPhotonEt)/pt;
        //auto relIso04 = (iso04.sumChargedHadronPt + iso04.sumNeutralHadronEt + iso04.sumPhotonEt)/pt;
         
        if ( relIso03 > 0.35 ) {
          continue;
        }

        if ( pt > 20 ) { 
          pt_gt20 = true;
        }

        if ( pt > 10 && pt < 20 ) {
          pt_gt10 = true;
        }
        
	my_muons.push_back(m);

    }

    if ( ! pt_gt20 || ! pt_gt10 ) {
       return false;
    }


    // Possible 4mu
    if ( my_muons.size() >= 4 ) {
      return true;
    }

    // Possible 2e2mu 
    if ( my_muons.size() >=2 && my_electrons.size() >= 2 ) {
      return true;
    }

    // Possible 4e
    if ( my_electrons.size() >= 4 ) {
       return true;
    }
	      
    return false;
}

void
ZZSkim::beginStream(edm::StreamID)
{}

void
ZZSkim::endStream() 
{}

// ------------ method called when starting to processes a run  ------------
/*
void
ZZSkim::beginRun(edm::Run const&, edm::EventSetup const&)
{ 
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
ZZSkim::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
ZZSkim::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
ZZSkim::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ZZSkim::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

DEFINE_FWK_MODULE(ZZSkim);
