#! /bin/sh

destroy() {
  rm -rf "fixtures"
  if [[ $? -ne 0 ]] ; then
    echo "\nError removing fixtures"
  elif [ -z $1 ] ; then
    echo "Fixtures destroyed"
  fi
}

create() {
  destroy quiet
  echo "Creating fixtues..."
  mkdir "fixtures"

  for i in {1..4}; do
    f=$(uuidgen | tr [:upper:] [:lower:] | sed 's/-//g').example
    let "c = 2 ** ($i * 4)"
    dd if=/dev/urandom of="fixtures/$f" bs=1 count=$c &>/dev/null
    echo "Fixture $f created with $c bytes"
  done
}

if [[ $1 == '-c' || $1 == '--create' ]]
then
  create
elif [[ $1 == '-d' || $1 == '--destroy' ]] 
then
  destroy
else
  echo 'No task specified:'
  echo '  -c, --create       Generate fixtures for testing'
  echo '  -d, --destroy      Delete fixtures from disk'
fi  


