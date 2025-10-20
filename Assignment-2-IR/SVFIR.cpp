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

    LLVMModuleSet::buildSVFModule(moduleNameVec);

    SVFIRBuilder builder;
    cout << "Generating SVFIR(PAG), call graph and ICFG ..." << endl;

    // TODO: here, generate SVFIR(PAG), call graph and ICFG to files
    //@{
    SVFIR *pag = builder.build();                    // SVF 3.2: 无参 build()

    // 1) SVFIR (PAG)
    pag->dump("svfir");                              // -> svfir.dot（写到当前目录）

    // 2) CallGraph（返回 const*，dump 非 const → const_cast）
    const CallGraph *cg_c = pag->getCallGraph();
    const_cast<CallGraph*>(cg_c)->dump("callgraph"); // -> callgraph.dot

    // 3) ICFG
    ICFG *icfg = pag->getICFG();
    icfg->dump("icfg", /*simple=*/false);            // -> icfg.dot
    //@}



    LLVMModuleSet::releaseLLVMModuleSet();
	return 0;
}