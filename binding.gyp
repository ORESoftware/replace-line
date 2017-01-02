{
  "targets": [
    {
      "target_name": "hello",
      "sources": [ "hello.cpp" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}