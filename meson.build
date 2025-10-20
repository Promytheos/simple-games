project('simple-games', 'c',
  version : '1.0.0',
  default_options : [
    'warning_level=2',
    'c_std=c99'
  ]
)

raylib_dep = dependency('raylib', required : true)
cc = meson.get_compiler('c')
math_dep = cc.find_library('m', required: false)

common_deps = [raylib_dep, math_dep]

main_src = 'src/main.c'

games = {
  'pong': 'src/pong.c',
  'tictactoe': 'src/tictactoe.c'
}

foreach name, impl : games
  executable(name,
    sources : [main_src, impl],
    dependencies : common_deps,
    install : true
  )
endforeach
