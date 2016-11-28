{
  "targets": [
    {
      # the name of the autotools build directory ('Build') is wired into
      # this spec.  Waddaya going to do?
      #
      "target_name": "cnctrq_client",
      "sources": [ "./src/cnctrq/cnctrq_client.cc" 
      		   , "./src/cnctrq/cnctrqClient.cc"
      		   , "./src/cnctrq/cnctrqConsistencyWorker.cc"
      		   , "./src/cnctrq/cnctrqDeleteWorker.cc"
      		   , "./src/cnctrq/cnctrqGetattrWorker.cc"
      		   , "./src/cnctrq/cnctrqListWorker.cc"
      		   , "./src/cnctrq/cnctrqMetaCopyWorker.cc"
      		   , "./src/cnctrq/cnctrqMkdirWorker.cc"
      		   , "./src/cnctrq/cnctrqShutdownWorker.cc"
      		   , "./src/cnctrq/cnctrqVPFindWorker.cc"
      		   , "./src/cnctrq/cnctrqVPPropertiesWorker.cc"
      		   , "./src/cnctrq/cnctrqWorkspaceWorker.cc"
      		   , "./src/cncSession.cc"
      		 ],
      # https://github.com/nodejs/node-gyp/issues/26
      "cflags_cc!": [ "-fno-exceptions", "-fno-rtti" ],
      "cflags_cc+": [ "-frtti" 
                      # following suppresses warnings from boost.
                      , "-Wno-unused-local-typedefs"
                      # following suppresses warnings from Pepsis.
                      , "-Wno-extra"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "src",
        #"/fast/Devel/cnc_server-nick/Build/src",
        "../../Build/src",
	"/opt/frqu/TOOLROOT/include",
	"/opt/frqu/TOOLROOT/include/pepsis",
	"/opt/frqu/TOOLROOT/include/frqu"
      ],
      # libtbbmalloc only works with LD_LIBRARY_PATH=/opt/frqu/TOOLROOT/lib64
      "libraries": [
        "-L/opt/frqu/TOOLROOT/lib64",
	"/opt/frqu/TOOLROOT/lib64/libVS-prtcl-cnctrq-cnc.a",
	"/opt/frqu/TOOLROOT/lib64/libVS-prtcl-cnctrq-mes.a",
	"/opt/frqu/TOOLROOT/lib64/libVS-prtcl.a",
	"/opt/frqu/TOOLROOT/lib64/libfrquCore.a",
	"../../../Build/src/rev/.libs/libcnc-rev.a",
	#"/opt/frqu/TOOLROOT/lib64/libtbbmalloc.so.2",
	"/opt/frqu/TOOLROOT/lib64/pepsis/libPepsis-peer.a",
	"/opt/frqu/TOOLROOT/lib64/pepsis/libPepsis.a",
	"/opt/frqu/TOOLROOT/lib64/libcpuid.a",
	"/opt/frqu/TOOLROOT/lib64/libev.a",
	"-ltbbmalloc",
	"-lboost_thread",
	"-lboost_system"
      ],
      "cflags": ["-g"]
    }
  ]
}
