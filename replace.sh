dirs=( include PRG src )
echo "Replace "$1" with "$2
find "${dirs[@]}" -type f -exec sed --debug -i 's/'$1'/'$2'/g' {} +
