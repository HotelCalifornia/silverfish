#!/usr/bin/env bash

rm index.zip

cd silverfish-intermediary

zip -X -r ../index.zip *

cd ..
aws lambda update-function-code --function-name silverfish-intermediary --zip-file fileb://index.zip
