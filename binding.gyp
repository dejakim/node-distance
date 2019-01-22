{
  "targets": [
    {
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      "target_name": "distance",
      "sources": [ "main.cpp" ]
    }
  ]
}
