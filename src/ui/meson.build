dep_gtk = dependency('gtk+-3.0')

install_headers('interface.h')

ui = static_library(
    'interface',
    'interface.c',
    install: true,
    dependencies: dep_gtk,  # Adicione as dependências aqui se necessário
)
