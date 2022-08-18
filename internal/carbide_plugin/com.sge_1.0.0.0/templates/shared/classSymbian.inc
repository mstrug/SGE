	<description>%template.desc</description>
	
	<wizardPage 
		id="other" 
		label="%other.label">
		<description>%other.desc</description>
		<textField 
				id="author"
				label="%other.author.label"
				persist="true">
				<description>%other.author.desc</description>
		</textField>
		<textField 
				id="version"
				label="%other.version.label"
				default="1.0">
				<description>%other.version.desc</description>
		</textField>
		<textField 
				id="copyright"
				label="%other.copyright.label"
				multiline="true"
				default="Your copyright notice"
				persist="true">
				<description>%other.copyright.desc</description>
		</textField>
	</wizardPage>
	
	<process class="com.nokia.carbide.cpp.project.ui.processes.CreateSourceFolders"
		 	bundle="com.nokia.carbide.cpp.project.ui">
		<parameter name="project" projectName="$(projectName)" />
		<parameter name="folder" path="$(sourceDir)" /> 
		<parameter name="folder" path="$(incDir)" forceUserInclude="true" /> 
	</process>
	

	<process class="com.nokia.carbide.cpp.project.core.processes.CopyFilesAndUpdateSymbianOSProjectFiles"
		 	bundle="com.nokia.carbide.cpp.project.core">
		<parameter name="project" projectName="$(projectName)" />
	
		<parameter name="file"
			sourcePath="ClassName.h" 
			targetPath="$(incDir)/$(incFileName)" 
			/> 
		<parameter name="file"
			sourcePath="ClassName.cpp" 
			targetPath="$(sourceDir)/$(srcFileName)" 
			/> 
	</process>
