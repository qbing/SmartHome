mod_SmartHome.la: mod_SmartHome.slo 
	$(SH_LINK) -rpath $(libexecdir) -module -avoid-version  mod_SmartHome.lo
DISTCLEAN_TARGETS = modules.mk
shared =  mod_SmartHome.la 
