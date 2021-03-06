#pragma once

#include <lib_init/DependentTask.hpp>
#include <lib_init/Base.hpp>
#include <pgpg_sampler/proxies/ScanPoseGenerator.hpp>
#include <pgpg_sampler/proxies/GraspPoseGenerator.hpp>
#include <pgpg_sampler/proxies/GraspOffsetGenerator.hpp>
#include <pgpg_sampler/proxies/Introspection.hpp>

namespace init
{

class PGPGSampler : public Base {

    
public:
    PGPGSampler(const std::string &scanPoseGeneratorName, const std::string &graspPoseGeneratorName, const std::string &graspOffsetGeneratorName, const std::string &introspectionName);
    virtual bool connect();
    void setDeployment(boost::shared_ptr<orocos_cpp::Deployment> deployment);
    
    DependentTask<pgpg_sampler::proxies::ScanPoseGenerator> scanPoseGeneratorTask;
    DependentTask<pgpg_sampler::proxies::GraspPoseGenerator> graspPoseGeneratorTask;
    DependentTask<pgpg_sampler::proxies::GraspOffsetGenerator> graspOffsetGeneratorTask;
    DependentTask<pgpg_sampler::proxies::Introspection> introspectionTask;
};

}