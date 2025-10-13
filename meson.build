project('Concorde', 'c', version : '0.1.0', default_options : ['warning_level=3', 'c_std=c11'])

src_dir = 'backend/src'
include_dir = join_paths('backend', 'include')
sqlite_dep = dependency('sqlite3', required: true)

sources = files()
foreach f : run_command('find', src_dir, '-name', '*.c', check : true).stdout().strip().split('\n')
  if f != ''
    sources += files(f)
  endif
endforeach

executable(
  'concorde',
  sources,
  dependencies: sqlite_dep,
  include_directories : include_directories(include_dir),
  install : true,
)