from glob import glob


def options(opt):
    opt.load('compiler_cxx')


def configure(cfg):
    cfg.load('compiler_cxx')


def build(bld):
    bld.objects(source='src/sbl/benchmark.cpp', target='benchmark')
    for i in glob('src/*.cpp'):
        bld.program(
            source=i,
            target=i + ".test",
            include='include',
            cxxflags=['-Wall', '-Werror', '-std=c++1y', '-Ofast'],
            use=['benchmark']
        )
