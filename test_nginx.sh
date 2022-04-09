docker rm -f test_nginx
docker run -it -p8000:80 --name test_nginx nginx bash # run `nginx` inside container