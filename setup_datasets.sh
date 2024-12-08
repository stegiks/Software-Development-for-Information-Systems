wget https://github.com/transactionalblog/sigmod-contest-2024/raw/refs/heads/main/dummy-data.bin
wget https://github.com/transactionalblog/sigmod-contest-2024/raw/refs/heads/main/dummy-queries.bin
mv dummy-data.bin data/
mv dummy-queries.bin data/
wget ftp://ftp.irisa.fr/local/texmex/corpus/siftsmall.tar.gz
tar -xvzf siftsmall.tar.gz
rm siftsmall.tar.gz
mv siftsmall/* data/
rmdir siftsmall