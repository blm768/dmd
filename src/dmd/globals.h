
/* Compiler implementation of the D programming language
 * Copyright (C) 1999-2019 by The D Language Foundation, All Rights Reserved
 * written by Walter Bright
 * http://www.digitalmars.com
 * Distributed under the Boost Software License, Version 1.0.
 * http://www.boost.org/LICENSE_1_0.txt
 * https://github.com/dlang/dmd/blob/master/src/dmd/globals.h
 */

#pragma once

#include "root/dcompat.h"
#include "root/ctfloat.h"
#include "root/outbuffer.h"
#include "root/filename.h"
#include "compiler.h"

// Can't include arraytypes.h here, need to declare these directly.
template <typename TYPE> struct Array;

typedef unsigned char Diagnostic;
enum
{
    DIAGNOSTICerror,  // generate an error
    DIAGNOSTICinform, // generate a warning
    DIAGNOSTICoff     // disable diagnostic
};

// The state of array bounds checking
typedef unsigned char CHECKENABLE;
enum
{
    CHECKENABLEdefault, // initial value
    CHECKENABLEoff,     // never do bounds checking
    CHECKENABLEon,      // always do bounds checking
    CHECKENABLEsafeonly // do bounds checking only in @safe functions
};

typedef unsigned char CHECKACTION;
enum
{
    CHECKACTION_D,        // call D assert on failure
    CHECKACTION_C,        // call C assert on failure
    CHECKACTION_halt,     // cause program halt on failure
    CHECKACTION_context   // call D assert with the error context on failure
};

enum CPU
{
    x87,
    mmx,
    sse,
    sse2,
    sse3,
    ssse3,
    sse4_1,
    sse4_2,
    avx,                // AVX1 instruction set
    avx2,               // AVX2 instruction set
    avx512,             // AVX-512 instruction set

    // Special values that don't survive past the command line processing
    baseline,           // (default) the minimum capability CPU
    native              // the machine the compiler is being run on
};

enum JsonFieldFlags
{
    none         = 0,
    compilerInfo = (1 << 0),
    buildInfo    = (1 << 1),
    modules      = (1 << 2),
    semantics    = (1 << 3)
};

enum CppStdRevision
{
    CppStdRevisionCpp98 = 199711,
    CppStdRevisionCpp11 = 201103,
    CppStdRevisionCpp14 = 201402,
    CppStdRevisionCpp17 = 201703
};

// Put command line switches in here
struct Param
{
    bool obj;           // write object file
    bool link;          // perform link
    bool dll;           // generate shared dynamic library
    bool lib;           // write library file instead of object file(s)
    bool multiobj;      // break one object file into multiple ones
    bool oneobj;        // write one object file instead of multiple ones
    bool trace;         // insert profiling hooks
    bool tracegc;       // instrument calls to 'new'
    bool verbose;       // verbose compile
    bool vcg_ast;       // write-out codegen-ast
    bool showColumns;   // print character (column) numbers in diagnostics
    bool vtls;          // identify thread local variables
    bool vgc;           // identify gc usage
    bool vfield;        // identify non-mutable field variables
    bool vcomplex;      // identify complex/imaginary type usage
    unsigned char symdebug;  // insert debug symbolic information
    bool symdebugref;   // insert debug information for all referenced types, too
    bool alwaysframe;   // always emit standard stack frame
    bool optimize;      // run optimizer
    bool map;           // generate linker .map file
    bool is64bit;       // generate 64 bit code
    bool isLP64;        // generate code for LP64
    bool isLinux;       // generate code for linux
    bool isOSX;         // generate code for Mac OSX
    bool isWindows;     // generate code for Windows
    bool isFreeBSD;     // generate code for FreeBSD
    bool isOpenBSD;     // generate code for OpenBSD
    bool isDragonFlyBSD;// generate code for DragonFlyBSD
    bool isSolaris;     // generate code for Solaris
    bool hasObjectiveC; // target supports Objective-C
    bool mscoff;        // for Win32: write COFF object files instead of OMF
    Diagnostic useDeprecated;
    bool stackstomp;    // add stack stomping code
    bool useUnitTests;  // generate unittest code
    bool useInline;     // inline expand functions
    bool useDIP25;      // implement http://wiki.dlang.org/DIP25
    bool noDIP25;       // revert to pre-DIP25 behavior
    bool release;       // build release version
    bool preservePaths; // true means don't strip path from source file
    Diagnostic warnings;
    bool pic;           // generate position-independent-code for shared libs
    bool color;         // use ANSI colors in console output
    bool cov;           // generate code coverage data
    unsigned char covPercent;   // 0..100 code coverage percentage required
    bool nofloat;       // code should not pull in floating point support
    bool ignoreUnsupportedPragmas;      // rather than error on them
    bool useModuleInfo; // generate runtime module information
    bool useTypeInfo;   // generate runtime type information
    bool useExceptions; // support exception handling
    bool betterC;       // be a "better C" compiler; no dependency on D runtime
    bool addMain;       // add a default main() function
    bool allInst;       // generate code for all template instantiations
    bool check10378;    // check for issues transitioning to 10738
    bool bug10378;      // use pre-bugzilla 10378 search strategy
    bool fix16997;      // fix integral promotions for unary + - ~ operators
                        // https://issues.dlang.org/show_bug.cgi?id=16997
    bool fixAliasThis;  // if the current scope has an alias this, check it before searching upper scopes
    bool vsafe;         // use enhanced @safe checking
    bool ehnogc;        // use @nogc exception handling
    bool dtorFields;        // destruct fields of partially constructed objects
                            // https://issues.dlang.org/show_bug.cgi?id=14246
    bool fieldwise;         // do struct equality testing field-wise rather than by memcmp()
    CppStdRevision cplusplus;  // version of C++ name mangling to support
    bool markdown;          // enable Markdown replacements in Ddoc
    bool vmarkdown;         // list instances of Markdown replacements in Ddoc
    bool showGaggedErrors;  // print gagged errors anyway
    bool printErrorContext;  // print errors with the error context (the error line in the source file)
    bool manual;            // open browser on compiler manual
    bool usage;             // print usage and exit
    bool mcpuUsage;         // print help on -mcpu switch
    bool transitionUsage;   // print help on -transition switch
    bool checkUsage;        // print help on -check switch
    bool checkActionUsage;  // print help on -checkaction switch
    bool revertUsage;       // print help on -revert switch
    bool previewUsage;      // print help on -preview switch
    bool externStdUsage;    // print help on -extern-std switch
    bool logo;              // print logo;

    CPU cpu;                // CPU instruction set to target

    CHECKENABLE useInvariants;     // generate class invariant checks
    CHECKENABLE useIn;             // generate precondition checks
    CHECKENABLE useOut;            // generate postcondition checks
    CHECKENABLE useArrayBounds;    // when to generate code for array bounds checks
    CHECKENABLE useAssert;         // when to generate code for assert()'s
    CHECKENABLE useSwitchError;    // check for switches without a default
    CHECKENABLE boundscheck;       // state of -boundscheck switch

    CHECKACTION checkAction;       // action to take when bounds, asserts or switch defaults are violated

    unsigned errorLimit;

    DArray<const char>  argv0;    // program name
    Array<const char *> *modFileAliasStrings; // array of char*'s of -I module filename alias strings
    Array<const char *> *imppath;     // array of char*'s of where to look for import modules
    Array<const char *> *fileImppath; // array of char*'s of where to look for file import modules
    const char *objdir;   // .obj/.lib file output directory
    const char *objname;  // .obj file output name
    const char *libname;  // .lib file output name

    bool doDocComments;  // process embedded documentation comments
    const char *docdir;  // write documentation file to docdir directory
    const char *docname; // write documentation file to docname
    Array<const char *> ddocfiles;  // macro include files for Ddoc

    bool doHdrGeneration;  // process embedded documentation comments
    const char *hdrdir;    // write 'header' file to docdir directory
    const char *hdrname;   // write 'header' file to docname
    bool hdrStripPlainFunctions; // strip the bodies of plain (non-template) functions

    bool doJsonGeneration;    // write JSON file
    const char *jsonfilename; // write JSON file to jsonfilename
    unsigned jsonFieldFlags;  // JSON field flags to include

    OutBuffer *mixinOut;                // write expanded mixins for debugging
    const char *mixinFile;             // .mixin file output name
    int mixinLines;                     // Number of lines in writeMixins

    unsigned debuglevel;   // debug level
    Array<const char *> *debugids;     // debug identifiers

    unsigned versionlevel; // version level
    Array<const char *> *versionids;   // version identifiers

    const char *defaultlibname; // default library for non-debug builds
    const char *debuglibname;   // default library for debug builds
    const char *mscrtlib;       // MS C runtime library

    const char *moduleDepsFile; // filename for deps output
    OutBuffer *moduleDeps;      // contents to be written to deps file

    // Hidden debug switches
    bool debugb;
    bool debugc;
    bool debugf;
    bool debugr;
    bool debugx;
    bool debugy;

    bool run;           // run resulting executable
    Strings runargs;    // arguments for executable

    // Linker stuff
    Array<const char *> objfiles;
    Array<const char *> linkswitches;
    Array<const char *> libfiles;
    Array<const char *> dllfiles;
    const char *deffile;
    const char *resfile;
    const char *exefile;
    const char *mapfile;
};

typedef unsigned structalign_t;
// magic value means "match whatever the underlying C compiler does"
// other values are all powers of 2
#define STRUCTALIGN_DEFAULT ((structalign_t) ~0)

struct Global
{
    const char *inifilename;
    const char *mars_ext;
    const char *obj_ext;
    const char *lib_ext;
    const char *dll_ext;
    const char *doc_ext;        // for Ddoc generated files
    const char *ddoc_ext;       // for Ddoc macro include files
    const char *hdr_ext;        // for D 'header' import files
    const char *json_ext;       // for JSON files
    const char *map_ext;        // for .map files
    bool run_noext;             // allow -run sources without extensions.

    const char *copyright;
    const char *written;
    Array<const char *> *path;        // Array of char*'s which form the import lookup path
    Array<const char *> *filePath;    // Array of char*'s which form the file import lookup path

    DArray<const char> version;     // Compiler version string
    const char *vendor;             // Compiler backend name

    Param params;
    unsigned errors;         // number of errors reported so far
    unsigned warnings;       // number of warnings reported so far
    unsigned gag;            // !=0 means gag reporting of errors & warnings
    unsigned gaggedErrors;   // number of errors reported while gagged
    unsigned gaggedWarnings; // number of warnings reported while gagged

    void* console;         // opaque pointer to console for controlling text attributes

    Array<class Identifier*>* versionids; // command line versions and predefined versions
    Array<class Identifier*>* debugids;   // command line debug versions and predefined versions

    /* Start gagging. Return the current number of gagged errors
     */
    unsigned startGagging();

    /* End gagging, restoring the old gagged state.
     * Return true if errors occurred while gagged.
     */
    bool endGagging(unsigned oldGagged);

    /*  Increment the error count to record that an error
     *  has occurred in the current context. An error message
     *  may or may not have been printed.
     */
    void increaseErrorCount();

    void _init();

    /**
    Returns: the version as the number that would be returned for __VERSION__
    */
    unsigned versionNumber();
};

extern Global global;

// Because int64_t and friends may be any integral type of the
// correct size, we have to explicitly ask for the correct
// integer type to get the correct mangling with dmd
#if __LP64__
// Be careful not to care about sign when using dinteger_t
// use this instead of integer_t to
// avoid conflicts with system #include's
typedef unsigned long dinteger_t;
// Signed and unsigned variants
typedef long sinteger_t;
typedef unsigned long uinteger_t;
#else
typedef unsigned long long dinteger_t;
typedef long long sinteger_t;
typedef unsigned long long uinteger_t;
#endif

typedef int8_t                  d_int8;
typedef uint8_t                 d_uns8;
typedef int16_t                 d_int16;
typedef uint16_t                d_uns16;
typedef int32_t                 d_int32;
typedef uint32_t                d_uns32;
typedef int64_t                 d_int64;
typedef uint64_t                d_uns64;

// file location
struct Loc
{
    const char *filename; // either absolute or relative to cwd
    unsigned linnum;
    unsigned charnum;

    Loc()
    {
        linnum = 0;
        charnum = 0;
        filename = NULL;
    }

    Loc(const char *filename, unsigned linnum, unsigned charnum);

    const char *toChars(bool showColumns = global.params.showColumns) const;
    bool equals(const Loc& loc) const;
};

enum LINK
{
    LINKdefault,
    LINKd,
    LINKc,
    LINKcpp,
    LINKwindows,
    LINKpascal,
    LINKobjc,
    LINKsystem
};

enum CPPMANGLE
{
    CPPMANGLEdefault,
    CPPMANGLEstruct,
    CPPMANGLEclass
};

enum MATCH
{
    MATCHnomatch,       // no match
    MATCHconvert,       // match with conversions
    MATCHconst,         // match with conversion to const
    MATCHexact          // exact match
};

enum PINLINE
{
    PINLINEdefault,      // as specified on the command line
    PINLINEnever,        // never inline
    PINLINEalways        // always inline
};

typedef uinteger_t StorageClass;
