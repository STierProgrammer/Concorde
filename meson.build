project('Concorde', 'c', version : '0.1.0', default_options : ['warning_level=3', 'c_std=c11'])

src_dir = 'src/'
backend_dir = 'src/server/'
client_dir = 'src/client/'
include_dir = 'include/'

sqlite_dep = dependency('sqlite3', required: true)
ncurses_dep = dependency('ncurses', required: true)

server_sources = files(
  join_paths(backend_dir, 'main.c'),
  join_paths(src_dir, 'sockapi.c'),
  join_paths(src_dir, 'netapi.c')
)

client_sources = files(
  join_paths(client_dir, 'main.c'),
  join_paths(src_dir, 'sockapi.c'),
  join_paths(src_dir, 'netapi.c')
)

executable(
  'concorde-client',
  client_sources,
  dependencies: [sqlite_dep, ncurses_dep],
  include_directories : include_directories(include_dir),
  install : true,
)

executable(
  'concorde-server',
  server_sources,
  dependencies: [sqlite_dep, ncurses_dep],
  include_directories : include_directories(include_dir),
  install : true,
)
