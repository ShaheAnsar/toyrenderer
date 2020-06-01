project('ogl', 'c' , 'cpp', version: '0.1', default_options: 'cpp_std=c++2a')
root_dir = '/home/shahe/linux-hdd/development/C++/OGL'
source_dir = join_paths(root_dir, 'src/')
sources_return = run_command('fd', '\.(cpp|c)$$', source_dir)
sources = sources_return.stdout().strip().split()
incdir = include_directories('include')
deps = [ dependency('glfw3') ]
executable('ogl.out', sources, include_directories:incdir, dependencies: deps,
  link_args: '-ldl')
