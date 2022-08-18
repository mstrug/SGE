<parameter name="project" projectName="$(projectName)" />

<!-- AIF DIR -->

<parameter name="file"
	sourcePath="aif/Basenameaif.rss" 
	targetPath="$(aifDir)/$(baseName)aif.rss" 
	/> 
<parameter name="file"
	sourcePath="aif/context_pane_icon.bmp" 
	targetPath="$(aifDir)/context_pane_icon.bmp" 
	substitute="false"
	/> 
<parameter name="file"
	sourcePath="aif/context_pane_icon_mask.bmp"
	targetPath="$(aifDir)/context_pane_icon_mask.bmp" 
	substitute="false"
	/> 
<parameter name="file"
	sourcePath="aif/list_icon.bmp" 
	targetPath="$(aifDir)/list_icon.bmp" 
	substitute="false"
	/> 
<parameter name="file"
	sourcePath="aif/list_icon_mask.bmp" 
	targetPath="$(aifDir)/list_icon_mask.bmp" 
	substitute="false"
	/> 
	
<!-- DATA DIR -->

<parameter name="file"
	sourcePath="data/Basename.rss" 
	targetPath="$(dataDir)/$(baseName).rss" 
	/> 
<parameter name="file"
	sourcePath="data/Basename_caption.rss" 
	targetPath="$(dataDir)/$(baseName)_caption.rss" 
	/> 
	
<!-- GROUP DIR -->
	
<parameter name="file"
	sourcePath="group/bld.inf" 
	targetPath="$(groupDir)/bld.inf" 
	/> 
<parameter name="file"
	sourcePath="group/Basename.mmp" 
	targetPath="$(groupDir)/$(baseName).mmp" 
	/> 
	
<!-- HELP DIR -->

<parameter name="file"
	sourcePath="help/Basename.cshlp" 
	targetPath="$(helpDir)/$(baseName).cshlp" 
	/>
<parameter name="file"
	sourcePath="help/Basename.xml" 
	targetPath="$(helpDir)/$(baseName).xml" 
	/>
<parameter name="file"
	sourcePath="help/build_help.mk" 
	targetPath="$(helpDir)/build_help.mk" 
	/>
<parameter name="file"
	sourcePath="help/Custom.xml" 
	targetPath="$(helpDir)/Custom.xml" 
	/>
