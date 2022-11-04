# .bashrc / .zshrc

camelcase() {
    perl -pe 's#(_|^)(.)#\u$2#g'
}

# from snake to camel :1,$s/_\([a-z]\)/\u\1/g 
# from camel to snake: :%s#\C\(\<\u[a-z0-9]\+\|[a-z0-9]\+\)\(\u\)#\l\1_\l\2#g 
