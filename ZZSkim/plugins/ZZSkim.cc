// -*- C++ -*-
//
// Package:    ZZProductionAODOutreachAnalysis/ZZSkim
// Class:      ZZSkim
// 
/**\class ZZSkim ZZSkim.cc ZZProductionAODOutreachAnalysis/ZZSkim/plugins/ZZSkim.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Thomas McCauley
//         Created:  Tue, 11 Jan 2022 12:23:27 GMT
//
//


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

class ZZSkim : public edm::stream::EDFilter<> {
   public:
      explicit ZZSkim(const edm::ParameterSet&);
      ~ZZSkim();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginStream(edm::StreamID) override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endStream() override;

      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------

      edm::InputTag electronInputTag_;
      edm::InputTag muonInputTag_;
 
      edm::EDGetTokenT<std::vector<pat::Electron> > electronToken_;
      edm::EDGetTokenT<std::vector<pat::Muon> > muonToken_;

};

ZZSkim::ZZSkim(const edm::ParameterSet& iConfig) :
electronInputTag_(iConfig.getParameter<edm::InputTag>("electronTag")),
muonInputTag_(iConfig.getParameter<edm::InputTag>("muonTag"))
{
        electronToken_ = consumes<std::vector<pat::Electron> >(electronInputTag_);
	muonToken_ = consumes<std::vector<pat::Muon> >(muonInputTag_);
	
}

ZZSkim::~ZZSkim()
{}

bool
ZZSkim::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   edm::Handle<std::vector<pat::Electron> > electrons;
   iEvent.getByToken(electronToken_, electrons);
 
   edm::Handle<std::vector<pat::Muon> > muons;
   iEvent.getByToken(muonToken_, muons);

   unsigned int nElectron = 0;
   unsigned int nMuon = 0;

   nElectron = electrons->size();
   nMuon = muons->size();

   if ( (nElectron+nMuon) < 4 )
	return false;

   for ( std::vector<pat::Electron>::const_iterator e = electrons->begin(), eEnd = electrons->end(); 
          e != eEnd; ++e )
    {   
	std::cout<<"electron"<<std::endl;

      std::cout<<"  pt, eta, phi, charge: "
	<< e->pt()
	<< e->eta()
	<< e->phi()
	<< e->charge() 
	<< std::endl;
 
      }


    for ( std::vector<pat::Muon>::const_iterator m = muons->begin(), mEnd = muons->end(); 
         m != mEnd; ++m )
  {   
	std::cout<<"muon"<<std::endl; 

	std::cout<<"  pt, eta, phi, charge: "
          << m->pt()
          << m->eta()
          << m->phi()
          << m->charge() 
          << std::endl;
  }

      

   return true;

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
