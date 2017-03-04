
# libwebler tests
> You will need python and Flask to use our testing

> Run these tests after installing `libwebler`

### Testing WebCrawler
From the directory of cloned repo

#### Start the server for testing
Run the below command in terminal

```$ make start_server```

This will start a `Flask` server on port `3000`

#### Start the test
In another terminal, run the below command

```$ make test```

You will receive `1024` callbacks as result from WebCrawler

#### Details of test
* The test server has `11` depths to crawl to get the final data.

* Starting URL is `http://localhost:3000/first/`

* Regex used for first `10` depth is

  `<\s*ul\s+[^>]*id\s*=\s*"wantedLinks1"[^>]*>([^?]*(?=</ul>))</ul>[^<]*<\s*ul\s+[^>]*id\s*=\s*"unwantedLinks"[^>]*>[^?]*(?=</ul>)</ul>[^<]*<\s*ul\s+[^>]*id\s*=\s*"wantedLinks2"[^>]*>([^?]*(?=</ul>))</ul>`

* Regex used for `11th`(last) depth is

  `<\s*ul\s+[^>]*id\s*=\s*"junkData"[^>]*>[^?]*(?=</ul>)</ul>[^<]*<\s*div\s+[^>]*id\s*=\s*"wantedData"[^>]*>([^?]*(?=</div>))</div>[^<]*<\s*ul\s+[^>]*id\s*=\s*"junkData2"[^>]*>[^?]*(?=</ul>)</ul>`

* Concurrency options are 3 depths and 5 threads in each depth.

* Every depth gets 2 more links for the next depth using first regex and a single capture of data at the last depth. Hence `2^10=1024` callbacks of final data will be received.
