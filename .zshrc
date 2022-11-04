# .bashrc / .zshrc

camelcase() {
    perl -pe 's#(_|^)(.)#\u$2#g'
}
