/**
 * SVFIR.cpp
 * @author kisslune
 */

#include "Graphs/SVFG.h"
#include "SVF-LLVM/SVFIRBuilder.h"

using namespace SVF;
using namespace llvm;
using namespace std;

int main(int argc, char** argv)
{
    int arg_num = 0;
    int extraArgc = 4;
    char** arg_value = new char*[argc + extraArgc];
    for (; arg_num < argc; ++arg_num) {
        arg_value[arg_num] = argv[arg_num];
    }
    std::vector<std::string> moduleNameVec;

    int orgArgNum = arg_num;
    arg_value[arg_num++] = (char*)"-model-arrays=true";
    arg_value[arg_num++] = (char*)"-pre-field-sensitive=false";
    arg_value[arg_num++] = (char*)"-model-consts=true";
    arg_value[arg_num++] = (char*)"-stat=false";
    assert(arg_num == (orgArgNum + extraArgc) && "more extra arguments? Change the value of extraArgc");

    moduleNameVec = OptionBase::parseOptions(arg_num, arg_value, "SVF IR", "[options] <input-bitcode...>");

    LLVMModuleSet::getLLVMModuleSet()->buildSVFModule(moduleNameVec);

    // Instantiate an SVFIR builder
    SVFIRBuilder builder;
    cout << "Generating SVFIR(PAG), call graph and ICFG ..." << endl;

    // TODO: here, generate SVFIR(PAG), call graph and ICFG, and dump them to files
    //@{
    SVFModule *svfModule = LLVMModuleSet::getLLVMModuleSet()->getSVFModule();

    // 1) 生成 SVFIR（又叫 PAG）
    SVFIR *pag = builder.build(svfModule);
    // 导出到 svfir.dot
    pag->dump("svfir");

    // 2) 生成/获取调用图并导出
    CallGraph *cg = pag->getCallGraph();
    cg->dump("callgraph");  // -> callgraph.dot

    // 3) 生成/获取 ICFG 并导出
    ICFG *icfg = pag->getICFG();
    icfg->dump("icfg");     // -> icfg.dot
    //@}

    return 0;
}
