#pragma once

#include <orocos_cpp/TransformerHelper.hpp>
#include <orocos_cpp/ConfigurationHelper.hpp>
#include <orocos_cpp/Deployment.hpp>
#include "DependentTask.hpp"

namespace RTT {
namespace corba {
class TaskContextProxy;
}
}

namespace init
{

class DependentTaskBase;

class Base
{
    friend class DependentTaskBase;
    
private:
    template <class TASK> void getProxyRecursive(const std::string &taskName, std::function<TASK*()> &fun)
    {
        if (fun)
        {
            return;
        }
        
        for (TaskWithConfig &taskWithConfig: allTasks)
        {
            std::cout << "init::Base::getProxyRecursive: " << taskWithConfig.task->getTaskName() << std::endl;
            if (taskWithConfig.task->getTaskName() == taskName)
            {
                std::cout << "init::Base::getProxyRecursive: proxy for " << taskName << " found." << std::endl;
                fun = [&]() {
                    return static_cast<TASK*>(taskWithConfig.proxy);
                };
                return;
            }
        }

        std::cout << "num dependencies: " << this->dependencies.size() << std::endl;
        for (init::Base* dependency: dependencies)
        {
            std::cout << "recursive.." << std::endl;
            dependency->getProxyRecursive<TASK>(taskName, fun);
        }

        std::cout << "stop.." << std::endl;
        return;
    }
    
    std::string name;
    
public:

    Base(const std::string &name);
    virtual ~Base();

    class TaskWithConfig
    {
    public:
        TaskWithConfig(DependentTaskBase *task) : task(task), proxy(nullptr)
        {
        }
        DependentTaskBase *task;
        RTT::corba::TaskContextProxy *proxy;
    };

    std::vector<TaskWithConfig> getAllTasks() {
        return allTasks;
    }

    std::vector<Base *> getDependencies()
    {
        return dependencies;
    };

    void updateDeploymentForTask(const std::string &taskName, orocos_cpp::Deployment *dpl);

    /***
     * Initializes the Proxes, for this Init Object
     * */
    virtual void initProxies();
    
    /***
     * @previous initProxies()
     * @following applyConfig();
     * 
     * Creates the connections between the components of this
     * Init Object.
     * */
    virtual bool connect();
    
    
    /***
     * @previous connect()
     * @following setupTransformer
     * 
     * Applies the configuraton for the dependend tasks of this 
     * Init Object.
     * */
    virtual bool applyConfig(orocos_cpp::ConfigurationHelper &confHelper);

    /***
     * @previous applyConfig
     * @following configure
     * 
     * Sets up the Transformer for the dependend tasks of this Init Object
     * */
    virtual bool setupTransformer(orocos_cpp::TransformerHelper &trHelper);

    /***
     * @previous setupTransformer
     * @following start
     * 
     * Configures the dependend tasks of this Init Object
     * */
    virtual bool configure();

    /***
     * @previous configure
     * @following -
     * 
     * Starts the dependend tasks of this Init Object
     * */
    virtual bool start();
    
    template <class TASK> std::function<TASK*()> getProxy(const std::string &taskName)
    {
        std::function<TASK*()> fun;
        getProxyRecursive<TASK>(taskName, fun);
        if (fun)
        {
            return fun;
        }
        
        throw std::runtime_error("init::Base::getProxy: no proxy for " + taskName + " found.");
    }

    const std::string &getName() {
        return name;
    };
protected:
    void registerTask(DependentTaskBase *task);

    void registerDependency(Base &dependency);

    std::vector<TaskWithConfig> allTasks;
    std::vector<Base *> dependencies;
};


}

